#include <stdint.h>
#include <string.h>

#include "app.h"
#include "tlc5971_show.h"

#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/sched.h"
#include "os/sleep.h"
#include "os/uart.h"
#include "os/pwm.h"
#include "os/adc.h"
#include "os/avg.h"
#include "driver/triac.h"
#include "driver/tlc5971.h"

/*
 * TODO: Buttons on PF4 (SW1) and PF0 (SW2)
 */

/*
 * Application descriptor: A place to store all of the state needed by your
 * application.
 */
struct app_descriptor {
    uint16_t tlc5971_tick;
    uint16_t adc_tick;
    struct sched_descriptor sched;
    struct task_descriptor tasks[TASK_ID_TOTAL];
    struct spi_descriptor spi;
    struct max31855_descriptor max31855;
    struct uart_descriptor uart_test;
    struct pwm_descriptor pwm_servo;
    struct tlc5971_descriptor tlc;
    struct avg_descriptor adc_avg;
    uint16_t bgr_buf[12];
    uint16_t timer_b;
    uint16_t timer_g;
};

struct app_descriptor app;
const char version_string[] = "PRIMORDIAL MITTENS";

void app_demo_timer_cb(void *ctx);

void
app_demo_timer_cb(void *ctx)
{
    uint16_t *timer = (uint16_t *)ctx;

    if (timer != NULL) {

        if (++(*timer) == 100) {
            *timer = 0;
        }

        if (timer == &(app.timer_b)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_B);
            }
            sched_set(&(app.sched), TASK_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));

        } else if (timer == &(app.timer_g)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_G);
            }
            sched_set(&(app.sched), TASK_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
        }
    }
}

/* Call once, don't poll */
void
app_demo_timer(void)
{
    sched_set(&(app.sched), TASK_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));
    sched_set(&(app.sched), TASK_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
}

void
app_init(void)
{
    memset((void *)&app, 0x00, sizeof(app));

    clock_init();
    tick_init();
    //sched_init(&(app.sched), &(app.tasks[0]), TASK_ID_TOTAL, SCHED_TIMER, SCHED_RESOLUTION);

    gpio_init(GPIO_LED_R);
    gpio_init(GPIO_LED_G);
    gpio_init(GPIO_LED_B);
    gpio_init(GPIO_TRIAC_IN);

    adc_init(ADC_0);

    avg_init(&(app.adc_avg), 64);

    spi_init(SPI_0, &(app.spi), 500000, 0);

    //max31855_init(&(app.max31855), SPI_ID_MAX31855, GPIO_NONE);

    //triac_init();
    //port_gpio_int_enable(GPIO_TRIAC_IN);

    //uart_init(UART_TEST, &(app.uart_test), 115200, "8N1");

    //app_demo_timer();
    //pwm_init(&(app.pwm_servo), PWM_SERVO, 330, 50);

    tlc5971_init(&(app.tlc), &(app.spi), 1);
}


void
app_demo(void)
{
    uint16_t adc_result;
    uint8_t i;

    if (tick_is_expired(&(app.tlc5971_tick))) {
        app.tlc5971_tick = tick_from_ms(20);

        adc_result = avg_get(&(app.adc_avg)) << 4;
        for (i=0; i<12; i++) {
            app.bgr_buf[i] = adc_result;
        }

        //tlc5971_show(&(app.bgr_buf[0]), 12);
        //tlc5971_testpattern(&(app.bgr_buf[0]), 12);
        tlc5971_set_bgr(&(app.tlc), &(app.bgr_buf[0]));

        gpio_toggle(GPIO_LED_R);

        //max31855_read(&app.max31855);
    }

    if (tick_is_expired(&(app.adc_tick))) {
        app.adc_tick = tick_from_ms(1);

        avg_moving(&(app.adc_avg), adc_sample(ADC_0));
    }


    //pwm_task(&app.pwm_servo);
}

void
app_demo_sleep_task(void)
{
    sleep();
}
