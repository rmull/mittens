#ifndef ADC_H_
#define ADC_H_

#include "port/port_adc.h"

struct adc_descriptor {
    uint16_t result;
    enum adc_hw_id id;
};


void adc_init(enum adc_hw_id adc);
uint16_t adc_sample(enum adc_hw_id adc);

#endif
