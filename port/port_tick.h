#ifndef PORT_TICK_H_
#define PORT_TICK_H_

#include <stdint.h>

#include "config.h"

/* Return the number of timer ticks in one millisecond */
uint32_t tick_port_ticks_per_ms(void);

/* Set the tick interval in milliseconds */
void tick_port_period_set(uint32_t period);

/* Register a tick interrupt handler */
void tick_port_callback_set(void (*cb)(void));

/* Start the tick timer */
void tick_port_start(void);

#endif
