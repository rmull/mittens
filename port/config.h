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
#define TICK_HZ         200

/***************** port_gpio.c/h ******************/
enum gpio_id {
    GPIO_ID_LED_R,
    GPIO_ID_LED_G,
    GPIO_ID_LED_B,
    GPIO_ID_TOTAL
};

/***************** port_timer.c/h **************/
/* Period of the post-scaled clock for the tickless timer */
#define TIMER_RESOLUTION     10000000        /* (1/TIMER_RESOLUTION) = 100ns */

/* Tickless timer names (timer.c/h) */
enum timer_id {
    TIMER_ID_LED_G,
    TIMER_ID_LED_B,
    TIMER_ID_TOTAL
};

/***************** port_spi.c/h ******************/
/* Name your SPI buses */
enum spi_id {
    SPI_ID_MAX31855,
    SPI_ID_TOTAL
};

/* Enumerate each MAX31855 in the system */
enum max31855_id {
    MAX31855_ID_0,
    MAX31855_ID_TOTAL
};

#endif
