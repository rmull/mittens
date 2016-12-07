#include <stdint.h>

#include "port_uart.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

void
uart_port_init(enum uart_id uart, uint32_t baud, char *mode_str)
{
    uint32_t mode = 0;

    if (mode_str[0] == '8') {
        mode |= UART_CONFIG_WLEN_8;
    }
    if (mode_str[1] == 'N') {
        mode |= UART_CONFIG_PAR_NONE;
    }
    if (mode_str[2] == '1') {
        mode |= UART_CONFIG_STOP_ONE;
    }
    mode = UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE;

    switch (uart) {
    case 1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
        GPIOPinConfigure(GPIO_PB0_U1RX);
        GPIOPinConfigure(GPIO_PB1_U1TX);
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTIntDisable(UART1_BASE, 0xFFFFFFFF);
        UARTIntClear(UART1_BASE, 0xFFFFFFFF);
        UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);
        UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), baud, mode);
        UARTEnable(UART1_BASE);
        break;

    default:
        break;
    }
}

void
uart_port_tx_byte(enum uart_id uart, uint8_t byte)
{
    switch (uart) {
    case 1:
        UARTCharPut(UART1_BASE, byte);
        break;
    default:
        break;
    }
}

#endif
