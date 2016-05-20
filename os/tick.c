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
    if (tick_sys == TICK_DISABLED) {
        tick_sys++;
    }
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
 * Period is defined by TICK_HZ at build-time.
 */
void
tick_init(void)
{
    tick_sys = 0x0000;

    tick_port_period_set(TICK_HZ);
    tick_port_callback_set(tick_interrupt);
    tick_start();
}

uint16_t
tick_from_ms(uint16_t ms)
{
    uint16_t ticks;

    ticks = (uint16_t)(((uint32_t)ms * TICK_HZ) / 1000);
    if (ticks >= 0x7FFF) {
        /*
         * If you get here, you need a slower system tick to achieve the
         * requested period. Note that we limit to one less tick than expected
         * in case we hit the TICK_DISABLED case below.
         */
        ticks = 0x7FFE;
    }

    /* Add the deadline to the current system tick */
    ticks += tick_sys;

    /*
     * If we hit the TICK_DISABLED case or the timer wraps, add an extra tick
     * because we skip a tick in the interrupt handler.
     */
    if (ticks == TICK_DISABLED || ticks < tick_sys) {
        ticks++;
    }

    return ticks;
}


/*
 * Returns 1 if the given timer counter is considered expired. Intended to be
 * used in a polling fashion by tasks that can tolerate some sloppiness in
 * periodicity.
 *
 * ASSUMPTIONS:
 * 1. All tick deadlines are set to be less than 0x8000 ticks in the future.
 * 2. We do not forget to poll the timer for 0x8000 ticks.
 */
uint8_t
tick_is_expired(uint16_t *alarm)
{
    /* Cast here because the subtraction may result in a promoted type */
    if (*alarm != TICK_DISABLED && (uint16_t)(tick_sys - *alarm) < 0x8000) {
        *alarm = TICK_DISABLED;
        return 1;
    }

    return 0;
}
