#include "port_tick.h"
#include "port_clock.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/systick.h"

/*
 * Specify period in milliseconds
 */
void
tick_port_period_set(uint32_t ms)
{
    SysTickPeriodSet(clock_port_ticks_per_ms() * ms);
}

void
tick_port_callback_set(void (*cb)(void))
{
    SysTickIntRegister(cb);
}

void
tick_port_start(void)
{
    SysTickIntEnable();
    SysTickEnable();
}
#endif
