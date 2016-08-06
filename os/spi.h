#ifndef SPI_H_
#define SPI_H_

#include "port/port_spi.h"
#include "serial.h"
#include "gpio.h"

struct spi_descriptor {
    struct serial_descriptor *tx;
    struct serial_descriptor *rx;
    uint32_t bitrate;
    enum gpio_id cs;
    uint8_t id;
    uint8_t mode;
};

/* Return values */
#define SPI_OK          0
#define SPI_BUSY        -1

void spi_init(enum spi_id id, struct spi_descriptor *sd);
int spi_read(struct spi_descriptor *spi, uint8_t *buf, uint16_t sz,
                                              void (*cb)(void *ctx), void *ctx);

#endif
