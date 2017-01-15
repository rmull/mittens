#ifndef PORT_UART_H_
#define PORT_UART_H_

#include "config.h"

enum uart_id {
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_TOTAL
};

void uart_port_init(enum uart_id, uint32_t, char *);
void uart_port_int_clear(enum uart_id);
void uart_port_tx_byte(enum uart_id, uint8_t);
void uart_port_tx_byte_nonblocking(enum uart_id, uint8_t);

#endif
