#ifndef SPI_H_
#define SPI_H_

#include "port/port_spi.h"
#include "serial.h"
#include "gpio.h"

struct spi_descriptor {
    struct serial_descriptor serial;
    uint32_t bitrate;
    enum gpio_id cs;
    uint8_t mode;
};

void spi_init(struct spi_descriptor *sd, enum spi_id id);
void spi_read(struct spi_descriptor *sd, uint8_t *buf, uint16_t sz);

#endif
