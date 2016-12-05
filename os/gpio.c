/*
 * gpio.c
 *
 * Provides handling of basic GPIO operations.
 *
 * Porting:
 * 1. Enumerate the system's GPIOs in port/gpio.h
 * 2. Map your application's GPIO needs to the underlying GPIO in app.h
 * 3. Fill in the switch statements in port/gpio.c
 */
#include "gpio.h"

void
gpio_set(enum gpio_id id, uint8_t level)
{
    if (level) {
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

uint8_t
gpio_get(enum gpio_id id)
{
    return port_gpio_get(id);
}
