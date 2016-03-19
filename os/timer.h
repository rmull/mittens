#ifndef TIMER_H_
#define TIMER_H_

#include "port/port_timer.h"

struct timer_descriptor {
    void (*cb)(void *ctx);
    void *ctx;
    uint16_t deadline;
    uint16_t pad;       // Unused padding - should we use it?
};

struct timer_module {
    struct timer_descriptor hires[TIMER_HIRES_ID_TOTAL];
    struct timer_descriptor lores[TIMER_LORES_ID_TOTAL];
    struct timer_descriptor *hires_next;
    struct timer_descriptor *lores_next;
};

void timer_init(void);
void timer_hires_set(enum timer_hires_id t, uint16_t deadline, void (*cb)(void *ctx), void *ctx);

#endif
