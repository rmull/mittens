#ifndef PORT_TIMER_H_
#define PORT_TIMER_H_

#include <stdint.h>

#include "config.h"

void timer_port_init();
void timer_port_start();
void timer_port_stop();
uint16_t timer_port_get_tick();
void timer_port_callback_set(void (*cb)(void));
void timer_port_set_load(uint16_t target);
uint16_t timer_port_get_load(void);
void timer_port_int_clear(void);

#endif
