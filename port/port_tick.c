#include "port_tick.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

/*
 * Note that this function refers to clock ticks, not the system tick that we
 * are generating from it.
 */
uint32_t
tick_port_ticks_per_ms(void)
{
    uint32_t hz;

    hz = SysCtlClockGet();

    return (hz / 1000);
}

/*
 * Specify period in milliseconds
 */
void
tick_port_period_set(uint32_t ms)
{
    SysTickPeriodSet(tick_port_ticks_per_ms() * ms);
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
