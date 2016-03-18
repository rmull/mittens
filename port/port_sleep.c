#include "port_sleep.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"

void
sleep_port(void)
{
    /* TODO: Fine-tune the peripheral enable/disable while in sleep mode */
    SysCtlSleep();
}

#endif
