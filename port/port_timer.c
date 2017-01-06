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
uint32_t timer_port_id_to_base(enum timer_hw_id timer);

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

uint32_t
timer_port_id_to_base(enum timer_hw_id timer)
{
    switch (timer) {
    case TIMER_0:
        return TIMER0_BASE;

    case TIMER_1:
        return TIMER1_BASE;

    case TIMER_2:
        return TIMER2_BASE;

    case TIMER_3:
        return TIMER3_BASE;

    default:
        return 0;
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
    uint32_t base = timer_port_id_to_base(timer);
    void (*fn)(void);

    switch (timer) {
    case TIMER_0:
        fn = timer_port_timer0_cb;
        break;

    default:
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
timer_port_start(enum timer_hw_id timer)
{
    uint32_t base = timer_port_id_to_base(timer);

    if (base != 0) {
        TimerEnable(base, TIMER_A);
    }
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
    uint32_t period = (clock_port_get_freq() / 330) - 1;
    uint32_t duty = (period * 50) / 100 - 1;

    switch (timer) {
    case TIMER_3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinConfigure(GPIO_PB2_T3CCP0);
        GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
        TimerControlLevel(TIMER3_BASE, TIMER_A, 1);  /* PWM active low */
        TimerPrescaleSet(TIMER3_BASE, TIMER_A, (period >> 16));
        TimerLoadSet(TIMER3_BASE, TIMER_A, (period & 0xFFFF));
        TimerPrescaleMatchSet(TIMER3_BASE, TIMER_A, (duty >> 16));
        TimerMatchSet(TIMER3_BASE, TIMER_A, (duty & 0xFFFF));
        TimerEnable(TIMER3_BASE, TIMER_A);
        break;

    default:
        break;
    }
}

void
timer_port_pwm_set_duty(enum timer_hw_id timer, uint32_t percent)
{
    uint32_t base = timer_port_id_to_base(timer);
    uint32_t duty = (clock_port_get_freq() * percent) / 100 - 1;

    if (base != 0) {
        TimerPrescaleMatchSet(base, TIMER_A, (duty >> 16) & 0xFFFF);
        TimerMatchSet(base, TIMER_A, (duty & 0xFFFF));
    }
}

void
timer_port_pwm_set_freq(enum timer_hw_id timer, uint32_t freq, uint32_t duty)
{
    uint32_t duty_new = (freq * duty) / 100 - 1;

    switch (timer) {
    case TIMER_3:
        //TimerDisable(TIMER3_BASE, TIMER_A);
        TimerPrescaleSet(TIMER3_BASE, TIMER_A, (freq >> 16));
        TimerLoadSet(TIMER3_BASE, TIMER_A, (freq & 0xFFFF));
        TimerPrescaleMatchSet(TIMER3_BASE, TIMER_A, (duty_new >> 16));
        TimerMatchSet(TIMER3_BASE, TIMER_A, (duty_new & 0xFFFF));
        //TimerEnable(TIMER3_BASE, TIMER_A);
        break;
    default:
        break;
    }
}

void
timer_port_set_freq(enum timer_hw_id timer, uint32_t hz)
{
    uint32_t base = timer_port_id_to_base(timer);

    if (base != 0) {
        TimerPrescaleSet(base, TIMER_A, (clock_port_get_freq() / hz) - 1);
        //TimerPrescaleSet(base, TIMER_A, (hz >> 16) & 0xFFFF);
        //TimerLoadSet(base, TIMER_A, (hz & 0xFFFF));
    }
}

uint32_t
timer_port_get_freq(enum timer_hw_id timer)
{
    uint32_t base = timer_port_id_to_base(timer);

    if (base != 0) {
        return clock_port_get_freq() / TimerPrescaleGet(timer_port_id_to_base(timer), TIMER_A);
    }

    return 0;
}

#endif
