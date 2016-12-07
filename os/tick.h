#ifndef TICK_H_
#define TICK_H_

#include <stdint.h>

#include "port/port_tick.h"

#define TICK_DISABLED   0xFFFF

void tick_init(void);
void tick_start(void);
uint16_t tick_from_ms(uint16_t ms);
uint8_t tick_is_expired(uint16_t *alarm);

/*
 * Usage Example:
 *
 * uint16_t alarm = 0;  // Timer
 *
 * tick_init();         // System tick initialized and started
 *
 * while (1) {
 *
 *     // alarm checked for expiration
 *     if (tick_is_expired(&alarm)) {
 *
 *         // Reschedule for 50ms in the future after every expiration
 *         alarm = tick_from_ms(50);
 *     }
 * }
 */

#endif
