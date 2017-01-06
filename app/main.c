#include "app.h"

int
main(void)
{
    app_init();

    while (1) {

        app_demo();

        //app_demo_sleep_task();
    }

    return 0;
}
