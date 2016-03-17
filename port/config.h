#ifndef CONFIG_H_
#define CONFIG_H_

/* Define your programming interface */
#define API_TIVAWARE    // TI TivaWare library
//#define API_HARMONY   // Microchip Harmony library
//#define API_TI_MSP430 // TI MSP430 direct register access

/* System tick period (tick.c/h) */
#define TICK_PERIOD_MS  10

/* GPIO names (gpio.c/h) */
enum gpio_id {
    GPIO_ID_LED_R,
    GPIO_ID_LED_G,
    GPIO_ID_LED_B,
    GPIO_ID_TOTAL
};

#endif
