#ifndef CONFIG_H_
#define CONFIG_H_

/* Define your programming interface */
#define API_TIVAWARE    // TI TivaWare library
//#define API_HARMONY   // Microchip Harmony library
//#define API_TI_MSP430 // TI MSP430 direct register access

/***************** port_clock.c/h ******************/
/* Main oscillator frequency */
#define CLOCK_HZ        40000000

/***************** port_tick.c/h ******************/
#define TICK_HZ         200


/***************** port_timer.c/h **************/
/* Period of the post-scaled clock for the tickless timer */
#define TIMER_RESOLUTION    10000000        /* (1/TIMER_RESOLUTION) = 100ns */
#define TRIAC_RESOLUTION    4000000         /* 60Hz AC -> 4MHz @ 16-bit is plenty */
#define TRIAC_LATCH_TICKS   100             /* Minimum number of on-ticks to latch the triac */
#define TRIAC_HALFWAVE_TICKS 33333          /* TRIAC_RESOLUTION * 0.00833333 */

/* Assign timer peripherals */
#ifdef API_TIVAWARE
#define TIMER_PERIPH        TIMER0_BASE
#define TIMER_TRIAC_PERIPH  TIMER1_BASE
#endif

/* Tickless timer names (timer.c/h) */
enum timer_id {
    TIMER_ID_LED_G,
    TIMER_ID_LED_B,
    TIMER_ID_TOTAL
};

/* Enumerate each MAX31855 in the system */
enum max31855_id {
    MAX31855_ID_0,
    MAX31855_ID_TOTAL
};

#endif
