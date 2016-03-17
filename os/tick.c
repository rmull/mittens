/*
 * tick.c
 *
 * Provides a periodic system tick for basic timekeeping functions.
 */

#include "tick.h"

/* Global variables */
uint16_t tick_sys;

/* Function prototypes */
void tick_interrupt(void);

/*
 * System tick interrupt handler.
 */
void
tick_interrupt(void)
{
    tick_sys++;
}

/*
 * Start the system tick timer peripheral.
 *
 * ASSUMES:
 * 1. tick_init() has already been called successfully.
 */
void
tick_start(void)
{
    tick_port_start();
}

/*
 * Initialize a periodic system tick timer peripheral and automatically start
 * it.
 *
 * Specify period in milliseconds.
 */
void
tick_init(void)
{
    tick_sys = 0x0000;

    tick_port_period_set(TICK_PERIOD_MS);
    tick_port_callback_set(tick_interrupt);
    tick_start();
}

/*
 * Returns 1 if the given timer counter is considered expired. Intended to be
 * used in a polling fashion by tasks that can tolerate some sloppiness in
 * periodicity.
 *
 * ASSUMPTIONS:
 * 1. All tick deadlines are set to be less than 0x8000 ticks in the future.
 *    With 1ms ticks (the fastest system tick currently supported) the limit is
 *    32.768 seconds.
 * 2. We do not accidentally forget to poll the timer for 0x8000 ticks.
 */
uint8_t
tick_is_expired(uint16_t alarm)
{
    /* Cast here because the subtraction may result in a promoted type */
    if ((uint16_t)(tick_sys - alarm) < 0x8000) {
        return 1;
    }

    return 0;
}
