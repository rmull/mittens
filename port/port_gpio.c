#include "port_gpio.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

/*
 * TODO: Use per-pin initialization so that various higher-level modules can
 * take care of initializing their own GPIOs
 */
void
port_gpio_init(enum gpio_id id)
{
    switch (id) {
    case 0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
        break;

    case 1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
        break;

    case 2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
        break;

    default:
        break;
    }
}

void
port_gpio_set(enum gpio_id id, uint8_t level)
{
    switch (id) {
    case 0:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, level);
        break;

    case 1:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, level);
        break;

    case 2:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, level);
        break;

    default:
        break;
    }
}

void
port_gpio_toggle(enum gpio_id id)
{
    uint32_t level;

    switch (id) {
    case 0:
        level = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
        level ^= GPIO_PIN_1;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, level);
        break;

    case 1:
        level = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2);
        level ^= GPIO_PIN_2;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, level);
        break;

    case 2:
        level = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3);
        level ^= GPIO_PIN_3;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, level ^ 1);
        break;

    default:
        break;
    }
}

#endif
