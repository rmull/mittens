#ifndef PWM_H_
#define PWM_H_

#include "port/port_timer.h"

struct pwm_descriptor {
    uint32_t hz;
    uint32_t duty;
    enum timer_hw_id timer;
};

void pwm_init(struct pwm_descriptor *pwm, enum timer_hw_id timer, uint32_t hz, uint32_t duty);
void pwm_set_duty(struct pwm_descriptor *pwm, uint32_t duty);
void pwm_set_freq(struct pwm_descriptor *pwm, uint32_t hz);
uint32_t pwm_get_freq(struct pwm_descriptor *pwm);

#endif
