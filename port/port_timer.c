#include "port_timer.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for timer.h
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "port_clock.h"

void
timer_port_hires_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, (clock_port_get_freq() / TIMER_HIRES_PERIOD) - 1);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 0x0000FFFF);
    TimerMatchSet(TIMER0_BASE, TIMER_A, 0x00000000);
}

void
timer_port_hires_callback_set(void (*cb)(void))
{
    TimerIntRegister(TIMER0_BASE, TIMER_A, cb);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void
timer_port_hires_start(void)
{
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void
timer_port_hires_stop(void)
{
    TimerDisable(TIMER0_BASE, TIMER_A);
}

uint16_t
timer_port_hires_get_tick(void)
{
    return (uint16_t)(TimerValueGet(TIMER0_BASE, TIMER_A) & 0x0000FFFF);
}

void
timer_port_hires_target(uint16_t target)
{
    TimerMatchSet(TIMER0_BASE, TIMER_A, (uint32_t)target);
}

void
timer_port_hires_int_clear(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_A);
}

#endif