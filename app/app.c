#include <string.h>

#include "app.h"

#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/timer.h"
#include "os/sleep.h"

struct app_descriptor app;
const char version_string[] = "PRIMORDIAL MITTENS";

void timer_cb(void *ctx);

void
timer_cb(void *ctx)
{
    uint16_t *timer;

    if (ctx != NULL) {
        timer = (uint16_t *)ctx;

        if (++(*timer) == 100) {
            *timer = 0;
        }

        if (timer == &(app.timer_b)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_ID_LED_B);
            }
            timer_set(TIMER_ID_LED_B, 50000, timer_cb, (void *)&(app.timer_b));
        } else if (timer == &(app.timer_g)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_ID_LED_G);
            }
            timer_set(TIMER_ID_LED_G, 25000, timer_cb, (void *)&(app.timer_g));
        }
    }
}

/* Call once, don't poll */
void
app_demo_timer(void)
{
    timer_set(TIMER_ID_LED_B, 50000, timer_cb, (void *)&(app.timer_b));
    timer_set(TIMER_ID_LED_G, 25000, timer_cb, (void *)&(app.timer_g));
}

void
app_demo_tick_task(void)
{
    if (tick_is_expired(&(app.tick_r))) {
        gpio_toggle(GPIO_ID_LED_R);
        app.tick_r = tick_from_ms(500);
    }
}

void
app_demo_max31855_task(void)
{
    max31855_task(&(app.max31855));
}

void
app_init(void)
{
    uint8_t i;

    memset((void *)&app, 0x00, sizeof(app));

    clock_init();
    tick_init();
    timer_init();

    for (i=0; i<GPIO_ID_TOTAL; i++) {
        gpio_init(i);
        gpio_set(i, 1);
    }

    app.tick_r = tick_from_ms(0);

    //max31855_init(&(app.max31855), 0xFF); /* Not using GPIO CS, so use invalid ID here */

    //app_demo_timer();
}

void
app_demo_sleep_task(void)
{
    sleep();
}
