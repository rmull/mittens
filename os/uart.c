#include "uart.h"

#include <stdlib.h>

void *int_ctx[UART_TOTAL];

int
uart_rx(struct uart_descriptor *u, uint8_t *buf, uint16_t sz)
{
    if (u == NULL || buf == NULL || sz == 0) {
        return UART_ERROR;
    }

    if (serial_set_buf(u->rx, buf, sz) != SERIAL_OK) {
        return UART_ERROR;
    }

    return UART_OK;
}

int
uart_tx(struct uart_descriptor *u, uint8_t *buf, uint16_t sz)
{
    if (u == NULL || buf == NULL || sz == 0) {
        return UART_ERROR;
    }

    if (serial_set_buf(u->tx, buf, sz) != SERIAL_OK) {
        return UART_ERROR;
    }

    return UART_OK;
}

int
uart_tx_set_cb(struct uart_descriptor *u, void (*cb)(void *ctx), void *ctx)
{
    if (u == NULL) {
        return UART_ERROR;
    }

    serial_set_cb(u->tx, cb, ctx);

    return UART_OK;
}

int
uart_rx_set_cb(struct uart_descriptor *u, void (*cb)(void *ctx), void *ctx)
{
    if (u == NULL) {
        return UART_ERROR;
    }

    serial_set_cb(u->rx, cb, ctx);

    return UART_OK;
}

