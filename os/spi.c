#include <stdlib.h>
#include <string.h>

#include "spi.h"

/*
 * TODO: Support SPI port sharing
 * TODO: Support TX and TX/RX half-duplex chains
 * TODO: Support full-duplex swaps
 */

void *int_ctx[SPI_ID_TOTAL];

void spi_tasks(struct spi_descriptor *spi);
void spi_int_0(void);

/*
 * TODO: Could eventually use this as part of a interrupt-free polled setup
 */
void
spi_tasks(struct spi_descriptor *spi)
{
    uint8_t *buf = NULL;
    uint8_t data;

    data = spi_port_read(spi->bus_id);

    if (spi->tx != NULL && serial_get_sz(spi->tx) > 0) {
        buf = serial_pop(spi->tx);
        if (buf != NULL) {
            spi_port_write(spi->bus_id, *buf);
        }

    } else if (spi->rx != NULL && serial_get_sz(spi->rx) > 0) {
        buf = serial_push(spi->rx, data);
        if (buf != NULL) {
            spi_port_write(spi->bus_id, 0xFF);
        }
    }

    if (buf == NULL) {
        spi_port_int_disable(spi->bus_id);
    }
}

void
spi_int_0(void)
{
    struct spi_descriptor *spi = (struct spi_descriptor *)int_ctx[0];

    spi_port_int_clear(spi->bus_id);

    if (spi != NULL) {
        spi_tasks(spi);
    }
}

void
spi_init(struct spi_descriptor *spi)
{
    spi_port_init(spi->bus_id, spi->bitrate, spi->mode);

    int_ctx[spi->bus_id] = (void *)spi;

    switch (spi->bus_id) {
    case 0:
        spi_port_callback_set(spi->bus_id, spi_int_0);
        break;

    default:
        break;
    }
}

/*
 * Clocks out dummy bytes in order to make a read
 */
int
spi_read(struct spi_descriptor *spi, uint8_t *buf, uint16_t sz, void (*cb)(void *ctx), void *ctx)
{
    if (serial_set_buf(spi->rx, buf, sz) == SERIAL_OK) {
        serial_set_cb(spi->rx, cb, ctx);

        gpio_set(spi->cs, 0);

        spi_port_write(spi->bus_id, 0xFF);

        return SPI_OK;
    }

    return SPI_BUSY;
}
