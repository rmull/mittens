#include <stdint.h>

#include "adc.h"

void
adc_init(enum adc_hw_id adc)
{
    adc_port_init(adc);
}

uint16_t
adc_sample(enum adc_hw_id adc)
{
    return (uint16_t)adc_port_sample(adc);
}
