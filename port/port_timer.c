#include "port_timer.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "port_clock.h"

/*
 * TODO: The triac stuff is polluting this file.
 */

volatile uint8_t triac_duty = 0;

void triac_cb(void);

void
timer_port_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER_PERIPH, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);
    TimerPrescaleSet(TIMER_PERIPH, TIMER_A,
                                (clock_port_get_freq() / TIMER_RESOLUTION) - 1);
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
timer_port_callback_set(void (*cb)(void))
{
    TimerIntRegister(TIMER_PERIPH, TIMER_A, cb);
    TimerIntEnable(TIMER_PERIPH, TIMER_TIMA_TIMEOUT);
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
timer_port_int_clear(void)
{
    TimerIntClear(TIMER_PERIPH, TIMER_A);
}

#endif
