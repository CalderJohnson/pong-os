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

/*********************
 * Public Kernel API *                                                         
 *********************/

// Draw a pixel to the display
void draw_pixel(uint16_t x, uint16_t y, enum VgaColor color) {
    DOUBLE_BUFFER[y * VGA_WIDTH + x] = color; //stored continguously in VRAM
}

// Draw a rectangle to the display
void draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, enum VgaColor color) {
    uint16_t x, y;
    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            draw_pixel(x, y, color);
        }
    }
}

// Clear the display
void clear() {
    memset(DOUBLE_BUFFER, (uint8_t)DARK_GREY, VGA_WIDTH * VGA_HEIGHT);
}

// Copy to the right buffer
void write() {
    memcpy(VRAM, DOUBLE_BUFFER, VGA_WIDTH * VGA_HEIGHT);
}

//TODO player scores displayed