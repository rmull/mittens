/*
 * TODO: Write some info
 */

#include <string.h>

#include "sched.h"

/* Function prototypes */
void sched_execute(struct task_descriptor *t);
void sched_process(struct sched_descriptor *s, uint16_t ticks);
void sched_cb(void *ctx);

/*
 * Copy callback and context into temporary storage so that a task can
 * reschedule itself without getting immediately wiped out after returning.
 */
void
sched_execute(struct task_descriptor *t)
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
sched_cb(void *ctx)
{
    struct sched_descriptor *s = (struct sched_descriptor *)ctx;

    /*
     * The elapsed ticks in the interrupt handler are equivalent to the previous
     * timer load value.
     */
    sched_process(s, timer_port_get_load());
}

/*
 * Initialize the tickless timer module. Call once at powerup. Doesn't
 * automatically start the underlying timer peripherals - this will happen
 * automatically when a task is scheduled.
 */
void
sched_init(struct sched_descriptor *s, struct task_descriptor *tasks, uint8_t sz, enum timer_hw_id timer, uint32_t hz)
{
    memset((void *)s, 0, sizeof(struct sched_descriptor) * sz);

    if (s == NULL || tasks == NULL || sz == 0) {
        while (1);
    }

    s->tasks = tasks;
    s->ntasks = sz;

    timer_port_init(timer);
    timer_port_set_freq(timer, hz);
    timer_port_callback_set(timer, sched_cb, (void *)s);
}

/*
 * Scans the timer table looking for executable tasks, and remembers the lowest
 * deadline it sees. After the table is scanned, it will reschedule the timer
 * to execute the task with the soonest deadline
 */
void
sched_process(struct sched_descriptor *s, uint16_t ticks)
{
    struct task_descriptor *t_next = NULL;
    struct task_descriptor *t = &(s->tasks)[0];

    for (uint8_t i=0; i<s->ntasks; i++) {
        if (t[i].cb != NULL) {
            if (t[i].deadline > ticks) {
                t[i].deadline -= ticks;
                if (t_next == NULL || t[i].deadline < t_next->deadline) {
                    t_next = &t[i];
                }
            } else {
                t[i].deadline = 0;
                sched_execute(&t[i]);
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
sched_set(struct sched_descriptor *s, uint8_t task, uint16_t deadline, void (*cb)(void *ctx), void *ctx)
{
    timer_port_stop();

    (s->tasks[task]).deadline = deadline;
    (s->tasks[task]).cb = cb;
    (s->tasks[task]).ctx = ctx;

    sched_process(s, timer_port_get_load() - timer_port_get_tick());
}
