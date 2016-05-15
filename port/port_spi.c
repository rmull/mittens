#include "port_spi.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"

/*
 * mode = polarity (MSb), phase (LSb)
 */
void
spi_port_init(enum spi_id spi, uint32_t bitrate, uint8_t mode)
{
    uint32_t base;

    switch (spi) {
    case 0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3);
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);

        GPIOPinConfigure(GPIO_PA2_SSI0CLK);
        GPIOPinConfigure(GPIO_PA3_SSI0FSS);
        GPIOPinConfigure(GPIO_PA4_SSI0RX);

        GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

        base = SSI0_BASE;
        break;

    default:
        break;
    }

    /*
     * This swap exists because TI has a different idea about mode numbers.
     */
    if (mode == 1) {
        mode = 2;
    } else if (mode == 2) {
        mode = 1;
    }

    SSIConfigSetExpClk(base, SysCtlClockGet(), mode, SSI_MODE_MASTER, bitrate, 8);
    SSIEnable(base);
}

void
spi_port_callback_set(enum spi_id id, void (*cb)(void))
{
    uint32_t dummy;

    switch (id) {
    case 0:
        SSIIntRegister(SSI0_BASE, cb);
        SSIIntDisable(SSI0_BASE, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR );
        SSIIntClear(SSI0_BASE, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR );

        /* Drain the FIFO */
        while(SSIDataGetNonBlocking(SSI0_BASE, &dummy));

        /* Switch TX interrupt to EOT interrupt */
        //HWREG(SSI0_BASE + SSI_O_CR1) |= SSI_CR1_EOT;

        /* Save this for right after the transaction is completed */
        //SSIIntEnable(SSI0_BASE, SSI_TXFF);
        break;

    default:
        break;
    }
}

void
spi_port_int_clear(enum spi_id id)
{
    switch (id) {
    case 0:
        SSIIntClear(SSI0_BASE, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR);
        break;
    default:
        break;
    }
}

void
spi_port_int_disable(enum spi_id id)
{
    switch (id) {
    case 0:
        SSIIntDisable(SSI0_BASE, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR);
        break;
    default:
        break;
    }
}

void
spi_port_write(enum spi_id id, uint8_t data)
{
    switch (id) {
    case 0:
        SSIDataPutNonBlocking(SSI0_BASE, data);
        SSIIntEnable(SSI0_BASE, SSI_TXFF);
        break;
    default:
        break;
    }
}

uint8_t
spi_port_read(enum spi_id id)
{
    uint32_t data;

    switch (id) {
    case 0:
        SSIDataGetNonBlocking(SSI0_BASE, &data);
        break;
    default:
        data = 0;
        break;
    }

    return data;
}

#endif
