#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "port/port_uart.h"

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

void
uart_init(enum uart_id id, struct uart_descriptor *u, uint32_t baud, char *mode)
{
    u->id = id;
    u->baud = baud;
    memcpy(u->mode, mode, 3);
    uart_port_init(u->id, u->baud, u->mode);

    while (1) {
        uart_port_tx_byte(u->id, 'a');
        uart_port_tx_byte(u->id, 'b');
    }
}
