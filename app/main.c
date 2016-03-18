#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/sleep.h"

const char version_string[] = "PRIMORDIAL MITTENS";

uint16_t led_tick;

int
main(void)
{
    clock_init();
    tick_init();
    gpio_init();

    while (1) {

        /* Demo the system tick */
        if (tick_is_expired(led_tick)) {
            gpio_toggle(GPIO_ID_LED_B);
            led_tick += (500 / TICK_PERIOD_MS);
        }

        /* Demo the sleep function */
        sleep();
    }

    return 0;
}
