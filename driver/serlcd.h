#ifndef SERLCD_H_
#define SERLCD_H_

#include <stdint.h>

#include "os/uart.h"

enum serlcd_id {
    SERLCD_0,
    SERLCD_TOTAL
};

#define SERLCD_CHARS_PER_ROW    16
#define SERLCD_ROWS             2
#define SERLCD_CHARS_TOTAL      (SERLCD_CHARS_PER_ROW*SERLCD_ROWS)

#define SERLCD_ROW0_POS         0
#define SERLCD_ROW1_POS         64

/* Control codes */
#define SERLCD_CTRL_DISPLAY     0xFE
#define SERLCD_CTRL_CONFIG      0x7C

/* Display controls */
#define SERLCD_DISP_CLEAR       0x01
#define SERLCD_DISP_CURSOR_POS  0x80

/* Config controls */
#define SERLCD_CFG_BACKLIGHT    0x80

struct serlcd_descriptor {
    struct uart_descriptor *uart;
};

void serlcd_init(struct serlcd_descriptor *, struct uart_descriptor *);
void serlcd_print_string(struct serlcd_descriptor *, char *);
void serlcd_control(struct serlcd_descriptor *, uint8_t);
void serlcd_set_cursor(struct serlcd_descriptor *, uint8_t);
void serlcd_print_decimal(struct serlcd_descriptor *, uint32_t);
void serlcd_set_backlight(struct serlcd_descriptor *, uint8_t);

#endif
