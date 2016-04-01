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
            timer_hires_set(TIMER_HIRES_ID_LED_B, 50000, timer_cb, (void *)&(app.timer_b));
        } else if (timer == &(app.timer_g)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_ID_LED_G);
            }
            timer_hires_set(TIMER_HIRES_ID_LED_G, 25000, timer_cb, (void *)&(app.timer_g));
        }
    }
}

/* Call once, don't poll */
void
app_demo_timer_hires(void)
{
    timer_hires_set(TIMER_HIRES_ID_LED_B, 50000, timer_cb, (void *)&(app.timer_b));
    timer_hires_set(TIMER_HIRES_ID_LED_G, 25000, timer_cb, (void *)&(app.timer_g));
}

void
app_demo_tick_task(void)
{
    if (tick_is_expired(app.tick_r)) {
        gpio_toggle(GPIO_ID_LED_R);
        app.tick_r += (1000 / TICK_PERIOD_MS);
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

    max31855_init(&(app.max31855), GPIO_ID_SPI_MAX31855_CS);

    app_demo_timer_hires();
}

void
app_demo_sleep_task(void)
{
    sleep();
}
