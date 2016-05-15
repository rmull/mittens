/*
 * gpio.c
 *
 * Provides handling of basic GPIO operations.
 *
 * Porting:
 * - Enumerate your GPIOs in port/config.h
 * - Expand the switch statements in port/gpio.c
 *
 * TODO:
 * - A full port might enumerate all GPIOs in the system by their pin number,
 *   and then #define the application pin name to the pin number on the MCU.
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
