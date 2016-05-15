#ifndef TIMER_H_
#define TIMER_H_

#include "port/port_timer.h"

struct timer_descriptor {
    void (*cb)(void *ctx);
    void *ctx;
    uint16_t deadline;
};

void timer_init(void);
void timer_set(enum timer_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx);

#endif
