/*
 * gpio.c
 *
 * Provides handling for basic GPIO operations
 *
 */
#include "gpio.h"

void
gpio_set(enum gpio_id gpio, uint8_t level)
{
    if (level > 1) {
        level = 1;
    }

    port_gpio_set(gpio, level);
}

void
gpio_toggle(enum gpio_id gpio)
{
    port_gpio_toggle(gpio);
}

void
gpio_init(void)
{
    port_gpio_init();
}
