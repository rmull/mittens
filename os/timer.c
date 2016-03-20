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

#include "gpio.h"

/* Global variables */
struct timer_module timer;

/* Function prototypes */
void timer_hires_execute(struct timer_descriptor *t);
void timer_hires_find_next(void);
void timer_hires_adjust(void);
void timer_hires_int(void);

void
timer_hires_execute(struct timer_descriptor *t)
{
    void (*cb)(void *ctx) = t->cb;
    void *ctx = t->ctx;

    t->cb = NULL;
    t->ctx = NULL;
    t->deadline = 0;

    if (cb != NULL) {
        cb(ctx);
    }
}

void
timer_hires_find_next(void)
{
    uint8_t i;
    struct timer_descriptor *t_next = NULL;

    for (i=0; i<TIMER_HIRES_ID_TOTAL; i++) {
        if (timer.hires[i].cb != NULL) {
            if (t_next == NULL || timer.hires[i].deadline < t_next->deadline) {
                t_next = &timer.hires[i];
            }
        }
    }

    if (t_next != NULL) {
        timer.hires_next = t_next;
        timer_port_hires_set_load(timer.hires_next->deadline);
        timer_port_hires_start();
    }
}

/* Interrupt handler for timeout interrupt */
void
timer_hires_int(void)
{
    timer_port_hires_int_clear();

    if (timer.hires_next != NULL) {
        timer_hires_execute(timer.hires_next);
    }

    timer_hires_find_next();
}

void
timer_init(void)
{
    memset((void *)&timer, 0, sizeof(timer));

    timer_port_hires_init();
    timer_port_hires_callback_set(timer_hires_int);

    /* Don't automatically start - wait for a task to be scheduled first */
}

void
timer_hires_adjust(void)
{
    uint16_t delta;
    uint8_t i;

    delta = timer_port_hires_get_load() - timer_port_hires_get_tick();

    for (i=0; i<TIMER_HIRES_ID_TOTAL; i++) {
        if (timer.hires[i].cb != NULL) {
            if (timer.hires[i].deadline > delta) {
                timer.hires[i].deadline -= delta;
            } else {
                timer_hires_execute(&timer.hires[i]);
            }
        }
    }
}

void
timer_hires_set(enum timer_hires_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx)
{
    timer_port_hires_stop();

    timer.hires[t].deadline = deadline;
    timer.hires[t].cb = cb;
    timer.hires[t].ctx = ctx;

    timer_hires_adjust();

    timer_hires_find_next();
}
