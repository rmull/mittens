#ifndef SPI_H_
#define SPI_H_

#include "port/port_spi.h"
#include "serial.h"
#include "gpio.h"

struct spi_descriptor {
    struct serial_descriptor serial;
    uint32_t bitrate;
    enum gpio_id cs;
    uint8_t bus_id;
    uint8_t mode;
};

void spi_init(struct spi_descriptor *sd);
void spi_read(struct spi_descriptor *spi, uint8_t *buf, uint16_t sz);

#endif
