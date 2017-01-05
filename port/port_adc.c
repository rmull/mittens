#include <stdint.h>

#include "port_adc.h"

#ifdef API_TIVAWARE
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

void
adc_port_init(enum adc_hw_id adc)
{
    switch (adc) {
    case ADC_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
        ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
        //ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 4);
        ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 | ADC_CTL_END);
        ADCSequenceEnable(ADC0_BASE, 0);
        break;

    default:
        break;
    }
}

uint32_t
adc_port_sample(enum adc_hw_id adc)
{
    uint32_t data;

    switch (adc) {
    case ADC_0:
        ADCProcessorTrigger(ADC0_BASE, 0);
        while (ADCBusy(ADC0_BASE) == true);
        ADCSequenceDataGet(ADC0_BASE, 0, &data);
        break;

    default:
        data = 0;
        break;
    }

    return data;
}

#endif
