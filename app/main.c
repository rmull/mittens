#include <stdlib.h>

#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/sleep.h"
#include "os/timer.h"

const char version_string[] = "PRIMORDIAL MITTENS";

uint16_t led_tick;

void timer_led_r_cb(void *ctx);

uint16_t count = 0;
void
timer_led_r_cb(void *ctx)
{
    if (ctx != NULL) {
        count++;
        if (count == 100) {
            gpio_toggle(GPIO_ID_LED_R);
            count = 0;
        }

        timer_hires_set(TIMER_HIRES_ID_LED_R, 50000, timer_led_r_cb, (void *)version_string);
    }
}

int
main(void)
{
    clock_init();
    tick_init();
    gpio_init();
    timer_init();

    /* Demo the high-resolution tickless timer */
    timer_hires_set(TIMER_HIRES_ID_LED_R, 50000, timer_led_r_cb, (void *)version_string);

    while (1) {

        /* Demo the system tick */
        if (tick_is_expired(led_tick)) {
            //gpio_toggle(GPIO_ID_LED_B);
            led_tick += (500 / TICK_PERIOD_MS);
        }

        /* Demo the sleep function */
//        sleep();
    }

    return 0;
}
