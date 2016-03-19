#include "port_tick.h"
#include "port_clock.h"

#ifdef API_TIVAWARE
#include "driverlib/systick.h"

/*
 * Specify period in milliseconds
 */
void
tick_port_period_set(uint32_t ms)
{
    SysTickPeriodSet((clock_port_get_freq() / 1000) * ms);
}

void
tick_port_callback_set(void (*cb)(void))
{
    /*
     * Note: On non-library ports, we'll need to store and call this callback
     * ourselves.
     */
    SysTickIntRegister(cb);
}

void
tick_port_start(void)
{
    SysTickIntEnable();
    SysTickEnable();
}
#endif
