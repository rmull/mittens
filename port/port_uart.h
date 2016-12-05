#ifndef PORT_UART_H_
#define PORT_UART_H_

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

void uart_port_init(enum uart_id id);

#endif
