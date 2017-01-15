#include <stdlib.h>

#include "serlcd.h"

/*
 * This is a driver for the SparkFun SerLCD module for interfacing with
 * character LCD displays via UART.
 *
 * Supported version: 2.5
 */

void
serlcd_control(struct serlcd_descriptor *sl, uint8_t ctrl_code)
{
    uart_tx_byte(sl->uart, SERLCD_CTRL_DISPLAY);
    uart_tx_byte(sl->uart, ctrl_code);
}

void
serlcd_init(struct serlcd_descriptor *sl, struct uart_descriptor *u)
{
    if (sl == NULL || u == NULL) {
        while(1);
    }

    sl->uart = u;

    serlcd_control(sl, SERLCD_DISP_CLEAR);
}

void
serlcd_set_cursor(struct serlcd_descriptor *sl, uint8_t pos)
{
    serlcd_control(sl, SERLCD_DISP_CURSOR_POS + pos);
}

void
serlcd_print_string(struct serlcd_descriptor *sl, char *str)
{
    uint8_t i;

    for (i=0; i<SERLCD_CHARS_TOTAL; i++) {
        if (str[i] != '\0') {
            uart_tx_byte(sl->uart, str[i]);
        } else {
            break;
        }
    }
}

void
serlcd_print_decimal(struct serlcd_descriptor *sl, uint32_t num)
{
    uint8_t i;
    uint8_t buf[5];

    for (i=1; i<=5; i++) {
        buf[5-i] = (uint8_t)((num % 10) + '0');
        num /= 10;
    }

    for (i=0; i<5; i++) {
        uart_tx_byte(sl->uart, buf[i]);
    }
}

void
serlcd_set_backlight(struct serlcd_descriptor *sl, uint8_t level)
{
    if (level > 29) {
        level = 29;
    }
    uart_tx_byte(sl->uart, SERLCD_CTRL_CONFIG);
    uart_tx_byte(sl->uart, SERLCD_CFG_BACKLIGHT+level);
}
