#include "app.h"

void
main(void)
{
    app_init();

    while (1) {

        app_demo_tick_task();
        //app_demo_max31855_task();

        app_demo_sleep_task();
    }
}
