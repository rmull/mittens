#include <stdlib.h>
#include <string.h>

#include "spi.h"

/*
 * TODO: Support SPI port sharing
 * TODO: Support SPI reads
 */

void *int_ctx[SPI_ID_TOTAL];

void spi_int_0(void);
void test_spi_cb(void *ctx);

void
spi_int_0(void)
{
    uint8_t *buf;
    uint8_t data;

    struct spi_descriptor *spi = (struct spi_descriptor *)int_ctx[0];

    spi_port_int_clear(spi->bus_id);

    if (spi != NULL) {

        data = spi_port_read(spi->bus_id);

        buf = serial_pop(&(spi->serial));

        if (buf != NULL) {
            spi_port_write(spi->bus_id, *buf);
        } else {
            spi_port_int_disable(spi->bus_id);
        }
    }
}

void
spi_init(struct spi_descriptor *sd)
{
    spi_port_init(sd->bus_id, sd->bitrate, sd->mode);


    int_ctx[sd->bus_id] = (void *)sd;

    switch (sd->bus_id) {
    case 0:
        spi_port_callback_set(sd->bus_id, spi_int_0);
        break;

    default:
        break;
    }
}

void
test_spi_cb(void *ctx)
{
    (void *)ctx;
}

/*
 * Clocks out dummy bytes in order to make a read
 */
void
spi_read(struct spi_descriptor *spi, uint8_t *buf, uint16_t sz)
{
    uint8_t *data;

    gpio_set(spi->cs, 0);

    memset(buf, 0xFF, sz);

    if (serial_set_buf(&(spi->serial), buf, sz) == SERIAL_OK) {
        serial_set_cb(&(spi->serial), test_spi_cb, NULL);
    }

    data = serial_pop(&(spi->serial));

    if (data != NULL) {
        spi_port_write(spi->bus_id, *data);
    }
}
