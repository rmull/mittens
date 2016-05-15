/*
 * timer.c
 *
 * Provides an implementation of a simple tickless timer system.
 *
 * Define your timer IDs in port/config.h. This will reserve a statically
 * allocated slot in the timer table for your timer. If you know what you're
 * doing, you're welcome to share a single slot between two timers that will
 * never be pending concurrently.
 *
 * Requirements: One one-shot timer for each tickless timer.
 *
 * Potential TODO:
 * - Get rid of the config.h requirement and the enumerated timer IDs.
 * - Keep track of the next available insertion location in the table
 * - Return the insertion location to the caller of timer_set()
 */
#include <string.h>

#include "timer.h"

#include "gpio.h"

/* Global variables */
struct timer_descriptor timer[TIMER_ID_TOTAL];

/* Function prototypes */
void timer_execute(struct timer_descriptor *t);
void timer_process(uint16_t ticks);
void timer_int(void);

/*
 * Copy callback and context into temporary storage so that a timer task can
 * reschedule itself without getting immediately wiped out after returning.
 */
void
timer_execute(struct timer_descriptor *t)
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
timer_int(void)
{
    timer_port_int_clear();

    /*
     * The elapsed ticks in the interrupt handler are equivalent to the previous
     * timer load value.
     */
    timer_process(timer_port_get_load());
}

/*
 * Initialize the tickless timer module. Call once at powerup. Doesn't
 * automatically start the underlying timer peripherals - this will happen
 * automatically when a task is scheduled.
 */
void
timer_init(void)
{
    memset((void *)&timer[0], 0, sizeof(timer));

    timer_port_init();
    timer_port_callback_set(timer_int);
}

/*
 * Scans the timer table looking for executable tasks, and remembers the lowest
 * deadline it sees. After the table is scanned, it will reschedule the timer
 * to execute the task with the soonest deadline
 */
void
timer_process(uint16_t ticks)
{
    struct timer_descriptor *t_next = NULL;
    uint8_t i;

    for (i=0; i<TIMER_ID_TOTAL; i++) {
        if (timer[i].cb != NULL) {
            if (timer[i].deadline > ticks) {
                timer[i].deadline -= ticks;
                if (t_next == NULL || timer[i].deadline < t_next->deadline) {
                    t_next = &timer[i];
                }
            } else {
                timer[i].deadline = 0;
                timer_execute(&timer[i]);
            }
        }
    }

    if (t_next != NULL) {
        timer_port_set_load(t_next->deadline);
        timer_port_start();
    }
}

/*
 * Call this to schedule a task. The deadline is given as a multiple of
 * TIMER_RESOLUTION tick durations. A timer callback (given by cb) can call this
 * function from itself safely.
 */
void
timer_set(enum timer_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx)
{
    timer_port_stop();

    timer[t].deadline = deadline;
    timer[t].cb = cb;
    timer[t].ctx = ctx;

    timer_process(timer_port_get_load() - timer_port_get_tick());
}
