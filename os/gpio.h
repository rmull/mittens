#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#include "port/port_gpio.h"

void gpio_init(void);
void gpio_set(enum gpio_id gpio, uint8_t level);
void gpio_toggle(enum gpio_id gpio);
uint8_t gpio_get(enum gpio_id gpio);

#endif
