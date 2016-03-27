#include <stdlib.h>
#include <string.h>

#include "spi.h"

/* TODO: https://expensivesmokeandradiopixies.wordpress.com/category/computing/embedded/ */

void
spi_int_0(void)
{
    spi_port_int_clear(0);
}

/*
 * Connect a SPI ID to a descriptor
 */
void
spi_init(struct spi_descriptor *sd, enum spi_id spi)
{
    spi_port_init(spi, sd->bitrate, sd->mode);
    gpio_set(sd->cs, 1);

    switch (spi) {
    case 0:
        spi_port_callback_set(spi, spi_int_0);
        break;

    default:
        break;
    }
}

/*
 * Clocks out dummy bytes in order to make a read
 */
void
spi_read(struct spi_descriptor *sd, uint8_t *buf, uint16_t sz)
{
    gpio_set(sd->cs, 0);

    memset(buf, 0xFF, sz);
    sd->serial.buf = buf;
    sd->serial.sz = sz;
    sd->serial.ctx = NULL;
    sd->serial.cb = NULL;

    /* TODO: Set callback, send byte */
}
