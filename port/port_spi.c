#include "port_spi.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

void
spi_port_init(enum spi_id spi, uint32_t bitrate, uint8_t mode)
{
    uint32_t base;

    switch (spi) {
    case 0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
        base = SSI0_BASE;
        break;

    default:
        break;
    }

    SSIConfigSetExpClk(base, SysCtlClockGet(), mode, SSI_MODE_MASTER, bitrate, 8);
    SSIEnable(base);
}

void
spi_port_callback_set(enum spi_id spi, void (*cb)(void))
{
    switch (spi) {
    case 0:
        /* TODO: This needs some tweaking. https://expensivesmokeandradiopixies.wordpress.com/category/computing/embedded/ */
        SSIIntRegister(SSI0_BASE, cb);
        SSIIntEnable(SSI0_BASE, SSI_RXOR);
        break;

    default:
        break;
    }
}

void
spi_port_int_clear(enum spi_id spi)
{
    switch (spi) {
    case 0:
        SSIIntClear(SSI0_BASE, SSI_RXTO|SSI_RXOR);
        break;

    default:
        break;
    }
}

#endif
