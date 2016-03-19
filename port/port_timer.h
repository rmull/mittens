#ifndef PORT_TIMER_H_
#define PORT_TIMER_H_

#include <stdint.h>

#include "config.h"

void timer_port_hires_init();
void timer_port_hires_start();
void timer_port_hires_stop();
uint16_t timer_port_hires_get_tick();
void timer_port_hires_callback_set(void (*cb)(void));
void timer_port_hires_target(uint16_t target);
void timer_port_hires_int_clear(void);

#endif
