#include <stdint.h>
#include <stdlib.h>

#include "port/port_timer.h"

#include "pwm.h"

void
pwm_set_freq(struct pwm_descriptor *pwm, uint32_t hz)
{
    pwm->hz = hz;
    timer_port_set_freq(pwm->timer, hz);
}

void
pwm_set_duty(struct pwm_descriptor *pwm, uint32_t duty)
{
    ;
}

void
pwm_init(struct pwm_descriptor *pwm, enum timer_hw_id timer, uint32_t hz, uint32_t duty)
{
    if (pwm == NULL || hz == 0) {
        while (1);
    }

    pwm->timer = timer;
    pwm->hz = hz;
    pwm->duty = duty;

    timer_port_pwm_init(timer);
    //timer_port_set_freq(timer, hz);
    //timer_port_pwm_set_duty(timer, duty);
    //timer_port_start(timer);
}

uint32_t
pwm_get_freq(struct pwm_descriptor *pwm)
{
    return 0;
}


static uint32_t match = 121211;
void
pwm_task(struct pwm_descriptor *pwm)
{
    timer_port_pwm_set_freq(pwm->timer, match);
}
