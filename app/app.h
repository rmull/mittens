#ifndef APP_H_
#define APP_H_

#include "os/adc.h"
#include "os/avg.h"
#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/sched.h"
#include "os/sleep.h"
#include "os/uart.h"
#include "os/pwm.h"
#include "driver/max31855.h"
#include "driver/serlcd.h"
#include "driver/triac.h"
#include "driver/tlc5971.h"

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
#define SPI_ID_TLC5971          SPI_0

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

/*
 * Application descriptor: A place to store all of the state needed by your
 * application.
 */
struct app_descriptor {
    uint16_t tlc5971_tick;
    uint16_t servo_tick;
    uint16_t adc_tick;
    uint16_t lcd_tick;
    struct sched_descriptor sched;
    struct task_descriptor tasks[TASK_ID_TOTAL];
    struct spi_descriptor spi;
    struct max31855_descriptor max31855;
    struct uart_descriptor uart_test;
    struct pwm_descriptor pwm_servo;
    struct tlc5971_descriptor tlc;
    struct avg_descriptor adc_avg;
    struct serlcd_descriptor serlcd;
    uint16_t bgr_buf[12];
    uint16_t timer_b;
    uint16_t timer_g;
};


void app_init(void);
void app_demo(void);
void app_demo_timer(void);
void app_demo_sleep_task(void);

#endif
