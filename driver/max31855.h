#ifndef MAX31855_H_
#define MAX31855_H_

#include "os/spi.h"
#include "os/serial.h"

enum max31855_fault {
    MAX31855_FAULT_NONE = 0,
    MAX31855_FAULT_OC   = (1<<0),
    MAX31855_FAULT_SCG  = (1<<1),
    MAX31855_FAULT_SCV  = (1<<2)
};

enum max31855_status {
    MAX31855_OK         = 0,
    MAX31855_BUSY       = -1
};

#define MAX31855_BUF_SZ 4

struct max31855_descriptor {
    struct spi_descriptor spi;
    struct serial_descriptor spi_rx;
    int16_t degc;
    uint16_t poll_period_ms;
    uint8_t buf[MAX31855_BUF_SZ];
};

void max31855_init(struct max31855_descriptor *max31855, enum gpio_id cs);
int max31855_read(struct max31855_descriptor *max31855);

#endif
