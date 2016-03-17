#include "port_sleep.h"

#ifdef API_TIVAWARE
#include <stdbool.h>    // Required for sysctl.h
#include "driverlib/sysctl.h"

void
sleep_port(void)
{
    ;
}
#endif
