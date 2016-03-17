#include "port_clock.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"

void
clock_port_init(void)
{
    ;
}

/*
 * Note that this function refers to clock ticks, not the system tick that we
 * are generating from it.
 */
uint32_t
clock_port_ticks_per_ms(void)
{
    uint32_t hz;

    hz = SysCtlClockGet();

    return (hz / 1000);
}
#endif
