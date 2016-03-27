/*
 * gpio.c
 *
 * Provides handling for basic GPIO operations
 *
 */
#include "gpio.h"

void
gpio_set(enum gpio_id id, uint8_t level)
{
    if (level > 1) {
        level = 1;
    }

    port_gpio_set(id, level);
}

void
gpio_toggle(enum gpio_id id)
{
    port_gpio_toggle(id);
}

void
gpio_init(enum gpio_id id)
{
    port_gpio_init(id);
}
