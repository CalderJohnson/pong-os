#include "keyboard.h"
#include "vga_driver.h"
#include "../x86/ports.h"
#include "../x86/isr.h"
#include "../kernel/type.h"
#include "../kernel/util.h"
#include "../kernel/main.h"

#define SC_MAX 57

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '\t', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t *regs) {
    //the PIC leaves the scancode in port 0x60
    uint8_t scancode = port_byte_in(0x60);
    if (scancode > SC_MAX) {
        return;
    }
    char letter = sc_ascii[(int)scancode];
    keyboard_input(letter); //pass user input to the game
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}