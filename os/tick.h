#ifndef TICK_H_
#define TICK_H_

#include <stdint.h>

#include "port/port_tick.h"

void tick_init(void);
void tick_start(void);
uint8_t tick_is_expired(uint16_t alarm);

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
 *     if (tick_is_expired(alarm)) {
 *
 *         // Reschedule for 50ms in the future after every expiration
 *         alarm += (50 / TICK_PERIOD_MS);
 *     }
 * }
 */

#endif
