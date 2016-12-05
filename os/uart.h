#ifndef UART_H_
#define UART_H_

#include "port/port_uart.h"
#include "serial.h"

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
    struct serial_descriptor *rx;
    struct serial_descriptor *tx;
    enum uart_id id;
};

#define UART_OK         0
#define UART_ERROR      -1

int uart_rx(struct uart_descriptor *u, uint8_t *buf, uint16_t sz);
int uart_tx(struct uart_descriptor *u, uint8_t *buf, uint16_t sz);
int uart_tx_set_cb(struct uart_descriptor *u, void (*cb)(void *ctx), void *ctx);
int uart_rx_set_cb(struct uart_descriptor *u, void (*cb)(void *ctx), void *ctx);

#endif
