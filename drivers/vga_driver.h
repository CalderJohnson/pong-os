#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H

#include "../kernel/type.h"

#define VGA_WIDTH  320
#define VGA_HEIGHT 200

enum VgaColor {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE
};

void draw_pixel(uint16_t x, uint16_t y, enum VgaColor color);
void draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, enum VgaColor color);
void write();
void clear();

#endif