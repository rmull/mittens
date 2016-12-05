#include "port_gpio.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "port_timer.h" /* TODO: Remove this */

void
port_gpio_init(enum gpio_id id)
{
    switch (id) {
    case GPIO_F1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
        break;

    case GPIO_F2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
        break;

    case GPIO_F3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
        break;

    case GPIO_A7:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
        break;

    default:
        break;
    }
}

void
port_gpio_set(enum gpio_id id, uint8_t level)
{
    switch (id) {
    case GPIO_F1:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, level);
        break;

    case GPIO_F2:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, level);
        break;

    case GPIO_F3:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, level);
        break;

    case 3:
        break;

    default:
        break;
    }
}

void
port_gpio_toggle(enum gpio_id id)
{
    switch (id) {
    case GPIO_F1:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,
                     GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) ^ GPIO_PIN_1);
        break;

    case GPIO_F2:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,
                     GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) ^ GPIO_PIN_2);
        break;

    case GPIO_F3:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,
                     GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3) ^ GPIO_PIN_3);
        break;

    default:
        break;
    }
}

uint8_t
port_gpio_get(enum gpio_id id)
{
    switch (id) {
    case GPIO_F1:
        return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
    case GPIO_F2:
        return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2);
    case GPIO_F3:
        return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3);
    default:
        return 0;
    }
}

/* TODO */
void
testfn(void)
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_7);

    timer_triac_port_start();
}

void
port_gpio_int_enable(enum gpio_id id)
{
    switch (id) {
    case GPIO_F1:
    case GPIO_F2:
    case GPIO_F3:
        break;

    case GPIO_A7:
        GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_RISING_EDGE);
        GPIOIntRegister(GPIO_PORTA_BASE, testfn);
        GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
        break;
    default:
        break;
    }
}

#endif
