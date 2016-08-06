#ifndef PORT_SPI_H_
#define PORT_SPI_H_

#include <stdint.h>

#include "config.h"

enum spi_id {
    SPI_0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_TOTAL
};

void spi_port_init(enum spi_id id, uint32_t bitrate, uint8_t mode);
void spi_port_callback_set(enum spi_id id, void (*cb)(void));
void spi_port_int_disable(enum spi_id id);
void spi_port_int_clear(enum spi_id id);
void spi_port_write(enum spi_id id, uint8_t data);
uint8_t spi_port_read(enum spi_id id);

#endif
