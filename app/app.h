#ifndef APP_H_
#define APP_H_

#include "driver/max31855.h"

struct app_descriptor {
    struct max31855_descriptor max31855;
    uint16_t timer_b;
    uint16_t timer_g;
    uint16_t tick_r;
};

void app_init(void);
void app_demo_timer(void);
void app_demo_tick_task(void);
void app_demo_max31855_task(void);
void app_demo_sleep_task(void);

#endif
