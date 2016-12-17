#ifndef PID_H_
#define PID_H_

#define T_CONST         1

struct pid_float_descriptor {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float err_prev;
    float err_sum;
    float err_sum_max;
    float err_sum_min;
};

void pid_float_init(struct pid_float_descriptor *pid, float kp, float ki,
                            float kd, float err_sum_max, float err_sum_min);
float pid_float(struct pid_float_descriptor *pid, uint16_t sample);

#endif
