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

void uart_port_init(enum uart_id uart, uint32_t baud, char *mode_str);
void uart_port_tx_byte(enum uart_id uart, uint8_t byte);

#endif
