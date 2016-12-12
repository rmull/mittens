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

/* Name your SPI buses */
#define SPI_ID_MAX31855         SPI_0

/* Name your UARTs */
#define UART_TEST               UART_1

/* Name your timer-based PWMs */
#define PWM_SERVO               TIMER_3

/* Name your tickless timer */
#define SCHED_TIMER             TIMER_0
#define SCHED_RESOLUTION        10000000        /* 100ns period */

/* Name your sched tasks */
enum task_id {
    TASK_ID_LED_G,
    TASK_ID_LED_B,
    TASK_ID_TOTAL
};

void app_init(void);
void app_demo(void);
void app_demo_timer(void);
void app_demo_sleep_task(void);

#endif
