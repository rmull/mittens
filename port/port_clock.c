#include "port_clock.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"

void
clock_port_init(void)
{
    /* Use the internal 16MHz oscillator and disable the main oscillator */
    //SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_MAIN_OSC_DIS);

    /* Alternatively, use the external 16MHz crystal */
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    /*
     * DEBUG: Halt here if we don't get the desired clock frequency after
     * setting up our clocking.
     */
    if (SysCtlClockGet() != CLOCK_HZ) {
        while (1);
    }
}

/*
 * Note that this function refers to clock ticks, not the system tick that we
 * are generating from it.
 */
uint32_t
clock_port_get_freq(void)
{
    return SysCtlClockGet();
}
#endif
