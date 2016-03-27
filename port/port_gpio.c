#include "port_gpio.h"

#ifdef API_TIVAWARE
#include "inc/tm4c123gh6pm.h"

/*
 * TODO: Use per-pin initialization so that various higher-level modules can
 * take care of initializing their own GPIOs
 */
void
port_gpio_init(void)
{
    /* Enable GPIO port F */
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    /* Set the RGB LED pins to digital outputs */
    GPIO_PORTF_DIR_R = 0x0e;
    GPIO_PORTF_DEN_R = 0x0e;
}

void
port_gpio_set(enum gpio_id gpio, uint8_t level)
{
    switch (gpio) {
    case GPIO_ID_LED_R:
        GPIO_PORTF_DATA_R &= (uint32_t)(~(1 << 1));
        GPIO_PORTF_DATA_R |= (uint32_t)(level << 1);
        break;

    case GPIO_ID_LED_B:
        GPIO_PORTF_DATA_R &= (uint32_t)(~(1 << 2));
        GPIO_PORTF_DATA_R |= (uint32_t)(level << 2);
        break;

    case GPIO_ID_LED_G:
        GPIO_PORTF_DATA_R &= (uint32_t)(~(1 << 3));
        GPIO_PORTF_DATA_R |= (uint32_t)(level << 3);
        break;

    default:
        break;
    }
}

void
port_gpio_toggle(enum gpio_id gpio)
{
    switch (gpio) {
    case GPIO_ID_LED_R:
        GPIO_PORTF_DATA_R ^= (1 << 1);
        break;

    case GPIO_ID_LED_B:
        GPIO_PORTF_DATA_R ^= (1 << 2);
        break;

    case GPIO_ID_LED_G:
        GPIO_PORTF_DATA_R ^= (1 << 3);
        break;

    default:
        break;
    }
}

#endif
