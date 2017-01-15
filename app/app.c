#include <stdint.h>
#include <string.h>

#include "app.h"
#include "tlc5971_show.h"

/*
 * TODO: Buttons on PF4 (SW1) and PF0 (SW2)
 */

struct app_descriptor app;
const char version_string[] = "PRIMORDIAL MITTENS";

void app_demo_timer_cb(void *ctx);

void
app_demo_timer_cb(void *ctx)
{
    uint16_t *timer = (uint16_t *)ctx;

    if (timer != NULL) {

        if (++(*timer) == 100) {
            *timer = 0;
        }

        if (timer == &(app.timer_b)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_B);
            }
            sched_set(&(app.sched), TASK_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));

        } else if (timer == &(app.timer_g)) {
            if (*timer == 0) {
                gpio_toggle(GPIO_LED_G);
            }
            sched_set(&(app.sched), TASK_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
        }
    }
}

/* Call once, don't poll */
void
app_demo_timer(void)
{
    sched_set(&(app.sched), TASK_ID_LED_B, 50000, app_demo_timer_cb, (void *)&(app.timer_b));
    sched_set(&(app.sched), TASK_ID_LED_G, 25000, app_demo_timer_cb, (void *)&(app.timer_g));
}

void
app_init(void)
{
    memset((void *)&app, 0x00, sizeof(app));

    clock_init();
    tick_init();
    //sched_init(&(app.sched), &(app.tasks[0]), TASK_ID_TOTAL, SCHED_TIMER, SCHED_RESOLUTION);

    gpio_init(GPIO_LED_R);
    gpio_init(GPIO_LED_G);
    gpio_init(GPIO_LED_B);
    gpio_init(GPIO_TRIAC_IN);

    adc_init(ADC_0);

    avg_init(&(app.adc_avg), 8192);

    spi_init(SPI_0, &(app.spi), 5000000, 0);

    //max31855_init(&(app.max31855), SPI_ID_MAX31855, GPIO_NONE);

    triac_init();
    //port_gpio_int_enable(GPIO_TRIAC_IN);

    uart_init(UART_TEST, &(app.uart_test), 9600, "8N1");

    serlcd_init(&(app.serlcd), &(app.uart_test));
    //serlcd_set_backlight(&(app.serlcd), 29);
    serlcd_print_string(&(app.serlcd), "ADC Value");

    //app_demo_timer();
    pwm_init(&(app.pwm_servo), PWM_SERVO, 330, 50);

    tlc5971_init(&(app.tlc), &(app.spi), 1);
}

uint16_t servo_last;
void
app_demo(void)
{
    uint16_t adc_result = avg_get(&(app.adc_avg)) << 4;

    if (tick_is_expired(&(app.adc_tick))) {
        app.adc_tick = tick_from_ms(1);

        avg_moving(&(app.adc_avg), adc_sample(ADC_0));
    }

    if (tick_is_expired(&(app.tlc5971_tick))) {
        app.tlc5971_tick = tick_from_ms(50);

        //for (i=0; i<12; i++) {
        //    app.bgr_buf[i] = adc_result;
        //}
        tlc5971_show(&(app.bgr_buf[0]), 4);
        //tlc5971_testpattern(&(app.bgr_buf[0]), 12);

        tlc5971_set_bgr(&(app.tlc), &(app.bgr_buf[0]));

        gpio_toggle(GPIO_LED_R);

        //max31855_read(&app.max31855);
    }

    if (tick_is_expired(&(app.servo_tick))) {
        app.servo_tick = tick_from_ms(30);

        /* Small deadband */
        /* TODO: Add deadband to PWM? */
        if ((adc_result > servo_last+16) || (adc_result < servo_last-16)) {
            pwm_set_freq(&(app.pwm_servo), 50000+(((190000-50000)/0xFFFF)*adc_result));
            servo_last = adc_result;
        }
    }

    if (tick_is_expired(&(app.lcd_tick))) {
        app.lcd_tick = tick_from_ms(500);
        serlcd_set_cursor(&(app.serlcd), SERLCD_ROW1_POS);
        serlcd_print_decimal(&(app.serlcd), adc_result);
    }
}

void
app_demo_sleep_task(void)
{
    sleep();
}
