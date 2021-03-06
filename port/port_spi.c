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

        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);

        GPIOPinConfigure(GPIO_PA2_SSI0CLK);
        GPIOPinConfigure(GPIO_PA3_SSI0FSS);
        GPIOPinConfigure(GPIO_PA4_SSI0RX);
        GPIOPinConfigure(GPIO_PA5_SSI0TX);

        GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

        base = SSI0_BASE;
        break;

    default:
        base = 0;
        break;
    }

     /* This swap exists because TI has a different idea about mode numbers. */
    if (mode == 2) {
        mode = 1;
    } else if (mode == 1) {
        mode = 2;
    }

    if (base != 0) {
        SSIConfigSetExpClk(base, SysCtlClockGet(), mode, SSI_MODE_MASTER, bitrate, 8);
        //SSIDMAEnable(base, SSI_DMA_RX|SSI_DMA_TX);
        SSIEnable(base);
    }
}

void
spi_port_callback_set(enum spi_id id, void (*cb)(void))
{
    uint32_t dummy;
    uint32_t base;

    switch (id) {
    case 0:
        base = SSI0_BASE;

        /* Switch TX interrupt to EOT interrupt */
        //HWREG(SSI0_BASE + SSI_O_CR1) |= SSI_CR1_EOT;

        /* Save this for right after the transaction is completed */
        //SSIIntEnable(SSI0_BASE, SSI_TXFF);
        break;

    default:
        base = 0;
        break;
    }

    if (base != 0) {
        SSIIntClear(base, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR );
        SSIIntDisable(base, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR );
        /* Drain the FIFO */
        while(SSIDataGetNonBlocking(base, &dummy));

        SSIIntRegister(base, cb);
    }
}

void
spi_port_int_clear(enum spi_id id)
{
    uint32_t base;

    switch (id) {
    case 0:
        base = SSI0_BASE;
        break;
    default:
        base = 0;
        break;
    }

    if (base != 0) {
        SSIIntClear(base, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR);
    }
}

void
spi_port_int_disable(enum spi_id id)
{
    uint32_t base;

    switch (id) {
    case 0:
        base = SSI0_BASE;
        break;
    default:
        base = 0;
        break;
    }

    if (base != 0) {
        SSIIntDisable(base, SSI_TXFF | SSI_RXFF | SSI_RXTO | SSI_RXOR);
    }
}

void
spi_port_write(enum spi_id id, uint8_t data)
{
    uint32_t base;

    switch (id) {
    case 0:
        base = SSI0_BASE;
        break;
    default:
        base = 0;
        break;
    }

    if (base != 0) {
        SSIDataPutNonBlocking(SSI0_BASE, data);
        SSIIntEnable(SSI0_BASE, SSI_TXFF);
    }
}

uint8_t
spi_port_read(enum spi_id id)
{
    uint32_t data;
    uint32_t base;

    switch (id) {
    case 0:
        base = SSI0_BASE;
        break;
    default:
        base = 0;
        data = 0;
        break;
    }

    if (base != 0) {
        SSIDataGetNonBlocking(base, &data);
    }

    return data;
}

#endif
