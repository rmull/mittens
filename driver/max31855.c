#include "max31855.h"

#include "os/spi.h"
#include "os/tick.h"

struct max31855_descriptor max31855[MAX31855_ID_TOTAL];

void
max31855_init(enum max31855_id id)
{
    /* TODO: I don't care for this... */
    switch (id) {
    case 0:
        max31855[id].spi.cs = GPIO_ID_SPI_MAX31855_CS;
        break;

    default:
        break;
    }

    max31855[id].spi.bitrate = 5000000;     /* MAX31855 max SCLK is 5MHz */
    max31855[id].spi.mode = 1;              /* CPOL = 0, CPHA = 1 */ /* TODO: Confirm */
    max31855[id].alarm = 0;
    max31855[id].poll_period_ms = 500;

    spi_init(&(max31855[id].spi), SPI_ID_MAX31855);
}

void
max31855_task(void)
{
    uint8_t i;

    for (i=0; i<MAX31855_ID_TOTAL; i++) {
        if (tick_is_expired(max31855[i].alarm)) {
            spi_read(&(max31855[i].spi), max31855[i].buf, MAX31855_BUF_SZ);

            max31855[i].alarm += (max31855[i].poll_period_ms / TICK_PERIOD_MS);
        }
    }
}
