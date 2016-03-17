#include "os/tick.h"
#include "os/gpio.h"

uint16_t led_tick;

void
main(void)
{
    tick_init();
    gpio_init();

    while (1) {

        /* Demo the system tick */
        if (tick_is_expired(led_tick)) {
            gpio_toggle(GPIO_ID_LED_B);
            led_tick += (500 / TICK_PERIOD_MS);
        }
    }
}
