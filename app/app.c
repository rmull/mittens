#include <stdint.h>
#include <string.h>

#include "app.h"

#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/sched.h"
#include "os/sleep.h"
#include "os/uart.h"
#include "os/pwm.h"
#include "driver/triac.h"

/*
 * TODO: Buttons on PF4 (SW1) and PF0 (SW2)
 */

/*
 * Application descriptor: A place to store all of the state needed by your
 * application.
 */
struct app_descriptor {
    uint16_t tick;
    struct sched_descriptor sched;
    struct task_descriptor tasks[TASK_ID_TOTAL];
    struct max31855_descriptor max31855;
    struct uart_descriptor uart_test;
    struct pwm_descriptor pwm_servo;
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
    sched_init(&(app.sched), &(app.tasks[0]), TASK_ID_TOTAL, SCHED_TIMER, SCHED_RESOLUTION);

    gpio_init(GPIO_LED_R);
    gpio_init(GPIO_LED_G);
    gpio_init(GPIO_LED_B);
    gpio_init(GPIO_TRIAC_IN);

    max31855_init(&(app.max31855), SPI_ID_MAX31855, GPIO_NONE);

    triac_init();
    port_gpio_int_enable(GPIO_TRIAC_IN);

    uart_init(UART_TEST, &(app.uart_test), 115200, "8N1");

    app_demo_timer();
    //pwm_init(&(app.pwm_servo), PWM_SERVO);
}

void
app_demo(void)
{
    if (tick_is_expired(&app.tick)) {
        app.tick = tick_from_ms(500);

        gpio_toggle(GPIO_LED_R);

        max31855_read(&app.max31855);
    }
}

void
app_demo_sleep_task(void)
{
    sleep();
}
