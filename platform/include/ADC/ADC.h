#ifndef D_ADC_H
#define D_ADC_H

#include <stdint.h>
#include "GPIO.h"

typedef uint32_t milivolts_t;

typedef struct {
    GPIO_Pin_t pin;
} ADC_Config_t;

typedef struct {
    ADC_Config_t config;
    bool initialized;
} ADC_Handle_t;

void ADC_Create(ADC_Handle_t * adcHandle, ADC_Config_t * adcConfig);
void ADC_Destroy(ADC_Handle_t * adcHandle);
void ADC_Read( ADC_Handle_t * adcHandle, milivolts_t * reading);

#endif /* D_ADC_H */