#ifndef CONFIG_H_
#define CONFIG_H_

/* Define your programming interface */
#define API_TIVAWARE    // TI TivaWare library
//#define API_HARMONY   // Microchip Harmony library
//#define API_TI_MSP430 // TI MSP430 direct register access

/***************** port_clock.c/h ******************/
/* Main oscillator frequency */
#define CLOCK_HZ        80000000

/***************** port_tick.c/h ******************/
#define TICK_PERIOD_MS  100

/***************** port_gpio.c/h ******************/
enum gpio_id {
    GPIO_ID_LED_R,
    GPIO_ID_LED_G,
    GPIO_ID_LED_B,
    GPIO_ID_SPI_MAX31855_CS,
    GPIO_ID_TOTAL
};

/***************** port_timer.c/h ******************/
/* Period of the post-scaled clock tick desired for the high-resolution timer */
#define TIMER_HIRES_PERIOD      10000000        /* 100ns */

/* High-resolution timer names (timer.c/h) */
enum timer_hires_id {
    TIMER_HIRES_ID_LED_G,
    TIMER_HIRES_ID_LED_B,
    TIMER_HIRES_ID_TOTAL
};

/* Low-resolution timer names (timer.c/h) */
enum timer_lores_id {
    TIMER_LORES_ID_TOTAL
};

/***************** port_spi.c/h ******************/
/* Name your SPI buses */
enum spi_id {
    SPI_ID_MAX31855,
    SPI_ID_TOTAL
};

#endif
