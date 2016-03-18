#include "port_clock.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"

void
clock_port_init(void)
{
    uint32_t hz;

    /* Use the internal 16MHz oscillator and disable the main oscillator */
    SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_MAIN_OSC_DIS);

    /* Alternatively, use the external 16MHz crystal */
    /* SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); */

    hz = SysCtlClockGet();

    /*
     * Halt here if we don't get the desired clock frequency after setting up
     * our clocking.
     */
    while (hz != CLOCK_HZ) {};
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
