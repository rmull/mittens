#ifndef PORT_GPIO_H_
#define PORT_GPIO_H_

#include <stdint.h>

#include "config.h"

enum gpio_id {
    GPIO_NONE,
    GPIO_A0,
    GPIO_A1,
    GPIO_A2,
    GPIO_A3,
    GPIO_A4,
    GPIO_A5,
    GPIO_A6,
    GPIO_A7,
    GPIO_B0,
    GPIO_B1,
    GPIO_B2,
    GPIO_B3,
    GPIO_B4,
    GPIO_B5,
    GPIO_B6,
    GPIO_B7,
    GPIO_C0,
    GPIO_C1,
    GPIO_C2,
    GPIO_C3,
    GPIO_C4,
    GPIO_C5,
    GPIO_C6,
    GPIO_C7,
    GPIO_D0,
    GPIO_D1,
    GPIO_D2,
    GPIO_D3,
    GPIO_D4,
    GPIO_D5,
    GPIO_D6,
    GPIO_D7,
    GPIO_E0,
    GPIO_E1,
    GPIO_E2,
    GPIO_E3,
    GPIO_E4,
    GPIO_E5,
    GPIO_E6,
    GPIO_E7,
    GPIO_F0,
    GPIO_F1,
    GPIO_F2,
    GPIO_F3,
};

void port_gpio_init(enum gpio_id id);
void port_gpio_set(enum gpio_id id, uint8_t level);
uint8_t port_gpio_get(enum gpio_id id);
void port_gpio_toggle(enum gpio_id id);
void port_gpio_int_enable(enum gpio_id id);

#endif
