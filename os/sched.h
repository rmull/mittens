#ifndef SCHED_H_
#define SCHED_H_

#include "port/port_timer.h"

struct task_descriptor {
    void (*cb)(void *ctx);
    void *ctx;
    uint16_t deadline;
};

struct sched_descriptor {
    struct task_descriptor *tasks;
    uint8_t ntasks;
};

void sched_init(struct sched_descriptor *s, struct task_descriptor *tasks, uint8_t count, enum timer_hw_id timer, uint32_t hz);
void sched_set(struct sched_descriptor *s, uint8_t task, uint16_t deadline, void (*cb)(void *ctx), void *ctx);

#endif
