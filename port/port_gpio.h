#ifndef PORT_GPIO_H_
#define PORT_GPIO_H_

#include <stdint.h>

#include "config.h"

void port_gpio_init(void);
void port_gpio_set(enum gpio_id gpio, uint8_t level);
void port_gpio_toggle(enum gpio_id gpio);

#endif
