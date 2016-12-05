#ifndef APP_H_
#define APP_H_

#include "driver/max31855.h"

/*
 * GPIO mapping: Assign a name to your GPIOs that has meaning within the context
 * of the application. Available names can be found in port/port_gpio.h.
 */
#define GPIO_LED_R      GPIO_F1
#define GPIO_LED_G      GPIO_F2
#define GPIO_LED_B      GPIO_F3
#define GPIO_TRIAC_IN   GPIO_A7

/*
 * Application descriptor: A place to store all of the state needed by your
 * application.
 */
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
