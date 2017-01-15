#ifndef UART_H_
#define UART_H_

#include "serial.h"
#include "port/port_uart.h"

/*
 * Transmission (tx) is usually done by formatting bytes in a buffer, pointing
 * the *tx serial_descriptor to that buffer, telling it the size, and initiating
 * the transmit.
 *
 * Reception is frequently done in one of two ways:
 *  - User wants to process every byte
 *  - User wants to receive a multi-byte chunk and reduce the number of
 *    callbacks (if using interrupt) or interrupts (if using DMA)
 */

/* For a half-duplex UART, either *rx or *tx may be null. */
struct uart_descriptor {
    uint32_t baud;
    struct serial_descriptor *rx;
    struct serial_descriptor *tx;
    enum uart_id id;
    char mode[3];
};

#define UART_OK         0
#define UART_ERROR      -1

int uart_rx(struct uart_descriptor *, uint8_t *, uint16_t);
int uart_tx(struct uart_descriptor *, uint8_t *, uint16_t);
void uart_tx_byte(struct uart_descriptor *, uint8_t);
int uart_tx_set_cb(struct uart_descriptor *, void (*)(void *), void *);
int uart_rx_set_cb(struct uart_descriptor *, void (*)(void *), void *);
void uart_init(enum uart_id, struct uart_descriptor *, uint32_t, char *);
void uart_tx_byte_nonblocking(struct uart_descriptor *, uint8_t);

#endif
