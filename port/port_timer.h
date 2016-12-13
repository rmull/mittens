#ifndef PORT_TIMER_H_
#define PORT_TIMER_H_

#include <stdint.h>

#include "config.h"

enum timer_hw_id {
    TIMER_0,
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
    TIMER_5,
    TIMER_HW_ID_TOTAL
};

void timer_port_init(enum timer_hw_id timer);
void timer_port_callback_set(enum timer_hw_id timer, void (*cb)(void *ctx), void *ctx);
void timer_triac_port_init();
void timer_port_start(enum timer_hw_id timer);
void timer_triac_port_start(void);
void timer_port_stop();
uint16_t timer_port_get_tick();
void timer_port_set_load(uint16_t target);
void timer_port_pwm_set_freq(enum timer_hw_id timer, uint32_t match);
void timer_triac_port_set_load(uint16_t target);
uint16_t timer_port_get_load(void);
void timer_port_int_clear(void);
void timer_port_pwm_init(enum timer_hw_id timer);
void timer_port_pwm_set_duty(enum timer_hw_id timer, uint32_t percent);
void timer_port_set_freq(enum timer_hw_id timer, uint32_t hz);
uint32_t timer_port_get_freq(enum timer_hw_id timer);

#endif
