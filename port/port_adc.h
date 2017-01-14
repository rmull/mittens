#ifndef PORT_ADC_H_
#define PORT_ADC_H_

#include <stdint.h>

#include "config.h"

enum adc_hw_id {
    ADC_0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
    ADC_6,
    ADC_7,
    ADC_TOTAL
};

void adc_port_init(enum adc_hw_id adc);
uint32_t adc_port_sample(enum adc_hw_id adc);

#endif
