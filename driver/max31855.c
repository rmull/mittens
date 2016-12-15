#include "max31855.h"

#include "os/spi.h"

void max31855_spi_cb(void *ctx);
int16_t max31855_get_degc(struct max31855_descriptor *max31855);
uint8_t max31855_get_fault(struct max31855_descriptor *max31855);

void
max31855_init(struct max31855_descriptor *max31855, enum spi_id spi, enum gpio_id cs)
{
    max31855->spi.cs = cs;
    max31855->spi.bitrate = 100000;    /* MAX31855 max SCLK is 5MHz */
    max31855->spi.mode = 1;             /* CPOL = 0, CPHA = 1 */
    max31855->spi.rx = &(max31855->spi_rx);
    max31855->poll_period_ms = 500;

    spi_init(spi, &(max31855->spi), 500000, 1);
}

/*
 * Returns a signed integer in units of 0.25 degrees Celsius. Note that calling
 * this on a buffer that is not done being written will result in erroneous
 * data.
 */
int16_t
max31855_get_degc(struct max31855_descriptor *max31855)
{
    uint16_t raw;

    raw = ((uint16_t)(max31855->buf[0]) << 8) | max31855->buf[1];

    raw >>= 2;

    if (raw & 0x2000) {
        raw |= 0xE000;
    }

    return (int16_t)raw;
}

uint8_t
max31855_get_fault(struct max31855_descriptor *max31855)
{
    return max31855->buf[3] &
                    (MAX31855_FAULT_OC|MAX31855_FAULT_SCG|MAX31855_FAULT_SCV);
}

void
max31855_spi_cb(void *ctx)
{
    struct max31855_descriptor *max31855 = (struct max31855_descriptor *)ctx;

    if (max31855_get_fault(max31855) == MAX31855_FAULT_NONE) {
        max31855->degc = max31855_get_degc(max31855);
    }
}

int
max31855_read(struct max31855_descriptor *max31855)
{
    if (spi_read(&(max31855->spi), max31855->buf, MAX31855_BUF_SZ,
                                         max31855_spi_cb, max31855) == SPI_OK) {
        return MAX31855_OK;
    }

    return MAX31855_BUSY;
}
