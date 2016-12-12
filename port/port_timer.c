#include <stdlib.h>

#include "port_timer.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "port_clock.h"

static void *int_ctx[TIMER_HW_ID_TOTAL];
static void (*int_cb[TIMER_HW_ID_TOTAL])(void *ctx);

void timer_port_timer0_cb(void);
void timer_port_timer1_cb(void);
void timer_port_timer2_cb(void);
void timer_port_timer3_cb(void);

/*
 * TODO: The triac stuff is polluting this file.
 */

volatile uint8_t triac_duty = 0;

void
timer_port_timer0_cb(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_A);

    if (int_cb[0] != NULL) {
        int_cb[0](int_ctx[0]);
    }
}

void
timer_port_timer1_cb(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_A);

    if (int_cb[1] != NULL) {
        int_cb[1](int_ctx[1]);
    }
}

void
timer_port_timer2_cb(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_A);

    if (int_cb[2] != NULL) {
        int_cb[2](int_ctx[2]);
    }
}

void
timer_port_timer3_cb(void)
{
    TimerIntClear(TIMER3_BASE, TIMER_A);

    if (int_cb[3] != NULL) {
        int_cb[3](int_ctx[3]);
    }
}

void triac_cb(void);

void
timer_port_init(enum timer_hw_id timer)
{
    switch (timer) {
    case TIMER_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);
        break;

    default:
        break;
    }

}

void
triac_cb(void)
{
    TimerIntClear(TIMER_TRIAC_PERIPH, TIMER_A);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
    TimerDisable(TIMER_TRIAC_PERIPH, TIMER_A);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
}

void
timer_triac_port_init(void)
{
    uint32_t period, duty;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB4_T1CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_4);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER_TRIAC_PERIPH, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerControlLevel(TIMER_TRIAC_PERIPH, TIMER_A, 1);  /* PWM active low */

    period = (clock_port_get_freq() / 115) - 1;
    duty = (period * triac_duty) / 100 - 1;

    TimerPrescaleSet(TIMER_TRIAC_PERIPH, TIMER_A, (period >> 16));
    TimerLoadSet(TIMER_TRIAC_PERIPH, TIMER_A, (period & 0xFFFF));

    TimerPrescaleMatchSet(TIMER_TRIAC_PERIPH, TIMER_A, (duty >> 16));
    TimerMatchSet(TIMER_TRIAC_PERIPH, TIMER_A, (duty & 0xFFFF));

    TimerIntRegister(TIMER_TRIAC_PERIPH, TIMER_A, triac_cb);

    TimerEnable(TIMER_TRIAC_PERIPH, TIMER_A);
//    TimerIntEnable(TIMER_TRIAC_PERIPH, TIMER_CAPA_EVENT);
}


void
timer_port_callback_set(enum timer_hw_id timer, void (*cb)(void *ctx), void *ctx)
{
    uint32_t base;
    void (*fn)(void);

    switch (timer) {
    case TIMER_0:
        base = TIMER0_BASE;
        fn = timer_port_timer0_cb;
        break;

    default:
        base = 0;
        fn = NULL;
        break;
    }

    if (base != 0) {
        int_ctx[timer] = ctx;
        int_cb[timer] = cb;
        TimerIntRegister(base, TIMER_A, fn);
        TimerIntEnable(base, TIMER_TIMA_TIMEOUT);
    }
}

void
timer_port_start(void)
{
    TimerEnable(TIMER_PERIPH, TIMER_A);
}

void
timer_triac_port_start(void)
{
    uint32_t period, duty;

    TimerDisable(TIMER_TRIAC_PERIPH, TIMER_A);
    TimerConfigure(TIMER_TRIAC_PERIPH, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerControlLevel(TIMER_TRIAC_PERIPH, TIMER_A, 1);  /* PWM active low */
    period = (clock_port_get_freq() / 115) - 1;
    duty = (period * triac_duty) / 100 - 1;

    TimerPrescaleSet(TIMER_TRIAC_PERIPH, TIMER_A, (period >> 16));
    TimerLoadSet(TIMER_TRIAC_PERIPH, TIMER_A, (period & 0xFFFF));
    TimerPrescaleMatchSet(TIMER_TRIAC_PERIPH, TIMER_A, (duty >> 16));
    TimerMatchSet(TIMER_TRIAC_PERIPH, TIMER_A, (duty & 0xFFFF));

    TimerEnable(TIMER_TRIAC_PERIPH, TIMER_A);
}

void
timer_port_stop(void)
{
    TimerDisable(TIMER_PERIPH, TIMER_A);
}

uint16_t
timer_port_get_tick(void)
{
    return (uint16_t)(TimerValueGet(TIMER_PERIPH, TIMER_A) & 0x0000FFFF);
}

void
timer_port_set_load(uint16_t target)
{
    TimerLoadSet(TIMER_PERIPH, TIMER_A, (uint32_t)target);
}

void
timer_triac_port_set_load(uint16_t target)
{
    TimerLoadSet(TIMER_TRIAC_PERIPH, TIMER_A, (uint32_t)target);
    TimerLoadSet(TIMER_TRIAC_PERIPH, TIMER_A, (uint32_t)target + TRIAC_LATCH_TICKS);
}

uint16_t
timer_port_get_load(void)
{
    return (uint16_t)(TimerLoadGet(TIMER_PERIPH, TIMER_A));
}

void
timer_port_pwm_init(enum timer_hw_id timer)
{
    switch (timer) {
    case TIMER_3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        GPIOPinConfigure(GPIO_PB2_T3CCP0);
        TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
        break;

    default:
        break;
    }
}

void
timer_port_set_freq(enum timer_hw_id timer, uint32_t hz)
{
    uint32_t base;

    switch (timer) {
    case TIMER_0:
        base = TIMER0_BASE;
        break;

    case TIMER_3:
        base = TIMER3_BASE;
        break;

    default:
        base = 0;
        break;
    }

    if (base != 0) {
        TimerPrescaleSet(base, TIMER_A, (clock_port_get_freq() / hz) - 1);
    }
}

uint32_t
timer_port_get_freq(enum timer_hw_id timer)
{
    uint32_t hz;

    switch (timer) {
    case TIMER_3:
        hz = clock_port_get_freq() / TimerPrescaleGet(TIMER3_BASE, TIMER_A);
        break;

    default:
        hz = 0;
        break;
    }

    return hz;
}

#endif
