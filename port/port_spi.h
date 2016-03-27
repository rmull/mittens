#ifndef PORT_SPI_H_
#define PORT_SPI_H_

#include <stdint.h>

#include "config.h"

void spi_port_init(enum spi_id spi, uint32_t bitrate, uint8_t mode);
void spi_port_callback_set(enum spi_id spi, void (*cb)(void));
void spi_port_int_clear(enum spi_id spi);

#endif
