#include <string.h>

#include "app.h"

#include "os/clock.h"
#include "os/tick.h"
#include "os/gpio.h"
#include "os/timer.h"
#include "os/sleep.h"
#include "driver/triac.h"

struct app_descriptor app;
const char version_string[] = "PRIMORDIAL MITTENS";

void app_demo_timer_cb(void *ctx);

void
app_demo_timer_cb(void *ctx)
{
    uint16_t *timer;

    if (ctx != NULL) {
        timer = (uint16_t *)ctx;

        if (++(*timer) == 100) {
            *timer = 0;
        }

        if (timer == &(app.timer_b)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_B);
            }
            timer_set(TIMER_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));
        } else if (timer == &(app.timer_g)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_G);
            }
            timer_set(TIMER_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
        }
    }
}

/* Call once, don't poll */
void
app_demo_timer(void)
{
    timer_set(TIMER_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));
    timer_set(TIMER_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
}

void
app_init(void)
{
    memset((void *)&app, 0x00, sizeof(app));

    clock_init();
    tick_init();
    timer_init();

    gpio_init(GPIO_LED_R);
    gpio_init(GPIO_LED_G);
    gpio_init(GPIO_LED_B);
    gpio_init(GPIO_TRIAC_IN);

    max31855_init(&(app.max31855), GPIO_NONE);

    triac_init();
    port_gpio_int_enable(GPIO_TRIAC_IN);
}

void
app_demo(void)
{
    if (tick_is_expired(&app.tick)) {
        app.tick = tick_from_ms(500);

        gpio_toggle(GPIO_LED_R);

        max31855_read(&app.max31855);
    }
}

void
app_demo_sleep_task(void)
{
    sleep();
}
