#ifndef MAX31855_H_
#define MAX31855_H_

#include "os/spi.h"

/* Enumerate each MAX31855 in the system */
enum max31855_id {
    MAX31855_ID_0,
    MAX31855_ID_TOTAL
};

#define MAX31855_BUF_SZ         4

struct max31855_descriptor {
    struct spi_descriptor spi;
    uint16_t alarm;
    uint16_t poll_period_ms;
    uint8_t buf[MAX31855_BUF_SZ];
};

void max31855_init(enum max31855_id id);
void max31855_task(void);

#endif
