#ifndef PORT_GPIO_H_
#define PORT_GPIO_H_

#include <stdint.h>

#include "config.h"

void port_gpio_init(enum gpio_id id);
void port_gpio_set(enum gpio_id id, uint8_t level);
void port_gpio_toggle(enum gpio_id id);

#endif
