#include <stdint.h>
#include <stdlib.h>

#include "pid.h"

void
pid_float_init(struct pid_float_descriptor *pid, float kp, float ki, float kd,
               float err_sum_max, float err_sum_min)
{
    if (pid == NULL) {
        while (1);
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->err_sum_min = err_sum_min;
    pid->err_sum_max = err_sum_max;
    pid->err_prev = 0;
    pid->setpoint = 0;
    pid->err_sum = 0;
}

/*
 * This implements a PID controller that is meant to be run at a fixed time
 * interval. It uses floating point values for accuracy and clarity. T_CONST
 * is used as a placeholder in case we ever want to consider making this a
 * variable-period PID controller.
 */
float
pid_float(struct pid_float_descriptor *pid, uint16_t sample)
{
    /* P */
    float err = pid->setpoint - sample;/* P */

    /* I */
    pid->err_sum += (err * T_CONST);

    /* Prevent integral windup by limiting the accumulation of error */
    if (pid->err_sum < pid->err_sum_min) {
        pid->err_sum = pid->err_sum_min;
    } else if (pid->err_sum > pid->err_sum_max) {
        pid->err_sum = pid->err_sum_max;
    }

    /* D */
    float deriv = (err - pid->err_prev) / T_CONST;

    pid->err_prev = err;

    return (pid->kp * err) + (pid->ki * pid->err_sum) + (pid->kd * deriv);
};
