#ifndef TIMER_H
#define TIMER_H

#include "../kernel/type.h"

#define CLOCK_SPEED 1193180

//semantic container for tracking time (in ms)
typedef uint64_t clock_t;

void init_timer(uint32_t freq);
void sleep(uint32_t ms);
void init_clock(clock_t *clock);
uint32_t restart_clock(clock_t *clock);
uint32_t get_elapsed_time_ms(clock_t *clock);

#endif