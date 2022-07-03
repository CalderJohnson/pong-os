#include "timer.h"
#include "../kernel/type.h"
#include "../kernel/util.h"
#include "../x86/isr.h"
#include "../x86/ports.h"

/******************************
 * Private functions and data *
 ******************************/

uint64_t tick = 0; 

static void timer_callback(registers_t *regs) {
    tick++;
    UNUSED(regs);
}

//returns the number of ms since the timer was initialized
uint64_t get_elapsed_millis() {
    return tick;
}

/**********************************
 * Public functions for the kernel*
 **********************************/

// Hold system for specified number of milliseconds
void sleep(uint32_t ms) {
    uint64_t start = get_elapsed_millis();
    while (get_elapsed_millis() - start < ms) {/*wait*/}
}

// Start the system timer
void init_timer(uint32_t freq) { 
    //register timer callback to the interrupt handler
    register_interrupt_handler(IRQ0, timer_callback);

    //get the PIT value: hardware clock at 1193180 Hz
    uint32_t divisor = CLOCK_SPEED / freq; //tick updates freq * per second
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

/*******************
 * Clock Functions *
 *******************/

// Initialize a clock
void init_clock(clock_t *clock) {
    *clock = get_elapsed_millis();
}

// Restart a clock, giving its previous elapsed time
uint32_t restart_clock(clock_t *clock) {
    uint32_t elapsed = get_elapsed_time_ms(clock);
    *clock = get_elapsed_millis();
    return elapsed;
}

// Get the number of miliseconds since the clock started
uint32_t get_elapsed_time_ms(clock_t *clock) {
    return get_elapsed_millis() - *clock;
}