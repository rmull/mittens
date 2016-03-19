/*
 * timer.c
 *
 * Provides an implementation of a simple tickless timer system.
 *
 * Two 16-bit timers are supported:
 * 1. High resolution (hundreds of ns), short duration (tens of ms)
 * 2. Low resolution (tens of ms), long duration (tens of minutes)
 *
 * Define your timer IDs in port/config.h. This will reserve a statically
 * allocated slot in the timer table for your timer. If you know what you're
 * doing, you're welcome to share a single slot between two timers that will
 * never be pending concurrently.
 *
 * TODO: Introduce a secondary timer with a resolution of 1ms (range:
 */
#include <string.h>

#include "timer.h"

/* Global variables */
struct timer_module timer;

/* Function prototypes */
struct timer_descriptor * timer_hires_find_next(void);
void timer_hires_int(void);

/*
 * TODO: What happens when curr_tick = lowest_delta?
 *
 * This works for a count-down timer - the delta calculation will need to invert
 * for a count-up timer.
 */
struct timer_descriptor *
timer_hires_find_next(void)
{
    uint8_t i;
    struct timer_descriptor *next = NULL;
    uint16_t lowest_delta = 0xFFFF;
    uint16_t delta;
    uint16_t curr_tick = timer_port_hires_get_tick();

    for (i=0; i<TIMER_HIRES_ID_TOTAL; i++) {
        if (timer.hires[i].cb != NULL) {
            delta = (uint16_t)(curr_tick - timer.hires[i].deadline);

            if (delta < lowest_delta) {
                lowest_delta = delta;
                next = &timer.hires[i];
            }
        }
    }

    return next;
}

/* Interrupt handler for timer match interrupt */
void
timer_hires_int(void)
{
    timer_port_hires_int_clear();

    if (timer.hires_next != NULL) {
        if (timer.hires_next->cb != NULL) {

            /*
             * Copy callback and context to temporary variable so that they can
             * reschedule themselves without getting immediately wiped out.
             */
            void (*cb)(void *ctx) = timer.hires_next->cb;
            void *ctx = timer.hires_next->ctx;

            timer.hires_next->cb = NULL;

            cb(ctx);
        }

        timer.hires_next = NULL;
    }

    timer.hires_next = timer_hires_find_next();

    if (timer.hires_next == NULL) {
        timer_port_hires_stop();
    } else {
        timer_port_hires_target(timer.hires_next->deadline);
    }
}

void
timer_init(void)
{
    memset((void *)&timer, 0, sizeof(timer));

    timer_port_hires_init();
    timer_port_hires_callback_set(timer_hires_int);

    /* Don't automatically start - wait for a task to be scheduled */
}

void
timer_hires_set(enum timer_hires_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx)
{
    timer_port_hires_stop();

    timer.hires[t].deadline = timer_port_hires_get_tick() - deadline;
    timer.hires[t].cb = cb;
    timer.hires[t].ctx = ctx;

    timer_hires_find_next();
    timer_port_hires_start();
}
