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
 * Requirements: One one-shot timer for each tickless timer.
 *
 * TODO: Introduce a low-resolution timer with a resolution of 1ms
 */
#include <string.h>

#include "timer.h"

#include "gpio.h"

/* Global variables */
struct timer_module timer;

/* Function prototypes */
void timer_hires_execute(struct timer_descriptor *t);
void timer_hires_process(uint16_t ticks);
void timer_hires_int(void);

/*
 * Copy callback and context into temporary storage so that a timer task can
 * reschedule itself without getting immediately wiped out after returning.
 */
void
timer_hires_execute(struct timer_descriptor *t)
{
    void (*cb)(void *ctx) = t->cb;
    void *ctx = t->ctx;

    t->cb = NULL;
    t->ctx = NULL;

    if (cb != NULL) {
        cb(ctx);
    }
}

/* Interrupt handler for timeout interrupt */
void
timer_hires_int(void)
{
    timer_port_hires_int_clear();

    /*
     * The elapsed ticks in the interrupt handler are equivalent to the previous
     * timer load value.
     */
    timer_hires_process(timer_port_hires_get_load());
}

/*
 * Initialize the tickless timer module. Call once at powerup. Doesn't
 * automatically start the underlying timer peripherals - this will happen
 * automatically when a task is scheduled.
 */
void
timer_init(void)
{
    memset((void *)&timer, 0, sizeof(timer));

    timer_port_hires_init();
    timer_port_hires_callback_set(timer_hires_int);
}

/*
 * Scans the timer table looking for executable tasks, and remembers the lowest
 * deadline it sees. After the table is scanned, it will reschedule the timer
 * to execute the task with the soonest deadline
 */
void
timer_hires_process(uint16_t ticks)
{
    struct timer_descriptor *t_next = NULL;
    uint8_t i;

    for (i=0; i<TIMER_HIRES_ID_TOTAL; i++) {
        if (timer.hires[i].cb != NULL) {
            if (timer.hires[i].deadline > ticks) {
                timer.hires[i].deadline -= ticks;
                if (t_next == NULL || timer.hires[i].deadline < t_next->deadline) {
                    t_next = &timer.hires[i];
                }
            } else {
                timer.hires[i].deadline = 0;
                timer_hires_execute(&timer.hires[i]);
            }
        }
    }

    if (t_next != NULL) {
        timer_port_hires_set_load(t_next->deadline);
        timer_port_hires_start();
    }
}

/*
 * Call this to schedule a task. The deadline is given as a multiple of
 * TIMER_HIRES_PERIOD. A timer callback (given by cb) can call this function
 * from itself safely.
 */
void
timer_hires_set(enum timer_hires_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx)
{
    timer_port_hires_stop();

    timer.hires[t].deadline = deadline;
    timer.hires[t].cb = cb;
    timer.hires[t].ctx = ctx;

    timer_hires_process(timer_port_hires_get_load() - timer_port_hires_get_tick());
}
