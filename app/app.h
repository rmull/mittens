#ifndef APP_H_
#define APP_H_

#include "driver/max31855.h"

struct app_descriptor {
    uint16_t tick;
    struct max31855_descriptor max31855;
    uint16_t timer_b;
    uint16_t timer_g;
};

void app_init(void);
void app_demo(void);
void app_demo_timer(void);
void app_demo_sleep_task(void);

#endif
