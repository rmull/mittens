#include "max31855.h"

#include "os/spi.h"
#include "os/tick.h"

void
max31855_init(struct max31855_descriptor *max31855, enum gpio_id cs)
{
    max31855->spi.bus_id = SPI_ID_MAX31855;
    max31855->spi.cs = cs;
    max31855->spi.bitrate = 5000;       /* MAX31855 max SCLK is 5MHz */ /* TODO: Set low for testing */
    max31855->spi.mode = 1;             /* CPOL = 0, CPHA = 1 *//* TODO: Confirm */
    max31855->alarm = 0;
    max31855->poll_period_ms = 500;

    spi_init (&(max31855->spi));
}

void
max31855_task(struct max31855_descriptor *max31855)
{
    if (tick_is_expired(max31855->alarm)) {
        spi_read(&(max31855->spi), max31855->buf, MAX31855_BUF_SZ);

        max31855->alarm += (max31855->poll_period_ms / TICK_PERIOD_MS);
    }
}