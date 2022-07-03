/* globals */
#ifndef MAIN_H
#define MAIN_H

#include "type.h"
#include "../drivers/vga_driver.h"

enum BallDirection {
    upLeft,
    upRight,
    downLeft,
    downRight
};

void keyboard_input(char key);

#endif