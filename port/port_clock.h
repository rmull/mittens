#ifndef PORT_CLOCK_H_
#define PORT_CLOCK_H_

#include <stdint.h>

#include "config.h"

void clock_port_init(void);
uint32_t clock_port_ticks_per_ms(void);

#endif
