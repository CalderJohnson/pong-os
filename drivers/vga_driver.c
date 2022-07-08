#include "../kernel/type.h"
#include "../kernel/util.h"
#include "vga_driver.h"

/****************
 * Private Data *
 ****************/

// The VGA buffer is located at 0xA0000 in memory
static volatile uint8_t* VRAM = (volatile uint8_t *)0xA0000;
// Double buffering
static volatile uint8_t * DOUBLE_BUFFER = (volatile uint8_t *)0x10000;

static const uint8_t DIGITS[10][9][8] = { //each glyph is a 8x9 bitmap
    {
        { 0, 0, 0, 1, 1, 0, 0, 0 }, //0
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0, 1, 0, 0, 0 }, //1
        { 0, 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //2
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //3
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 0, 0, 1, 0, 0 }, //4
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //5
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //6
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //7
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //8
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 1, 1, 1, 0, 0 }, //9
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0, 0 }
    },
};

/*********************
 * Public Kernel API *                                                         
 *********************/

// Draw a pixel
void draw_pixel(uint16_t x, uint16_t y, enum VgaColor color) {
    DOUBLE_BUFFER[y * VGA_WIDTH + x] = color; //stored continguously in VRAM
}

// Draw a rectangle
void draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, enum VgaColor color) {
    uint16_t x, y;
    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            draw_pixel(x, y, color);
        }
    }
}

// Draw a digit glyph
void draw_digit(uint16_t x, uint16_t y, enum VgaColor color, uint8_t digit) {
    if (!(digit >= 0 && digit <= 9)) {
        return;
    }
    for (uint8_t y_offset = 0; y_offset < 9; y_offset++) {
        for (uint8_t x_offset = 0; x_offset < 8; x_offset++) {
            if (DIGITS[digit][y_offset][x_offset])
                draw_pixel(x + x_offset, y + y_offset, color);
        }
    }
}

void draw_scoreboard(struct ScoreBoard *scoreboard) {
    draw_digit(5, 5, BLUE, scoreboard->playerScore);
    draw_digit(310, 5, RED, scoreboard->enemyScore);
}

// Clear the display
void clear() {
    memset(DOUBLE_BUFFER, (uint8_t)DARK_GREY, VGA_WIDTH * VGA_HEIGHT);
}

// Copy to the right buffer
void write() {
    memcpy(VRAM, DOUBLE_BUFFER, VGA_WIDTH * VGA_HEIGHT);
}