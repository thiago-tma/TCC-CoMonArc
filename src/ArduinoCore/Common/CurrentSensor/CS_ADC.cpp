#include <Arduino.h>
#include <CS_ADC.h>
#include <BSP_Pins.h>
#include <GPIO.h>

static GPIO_Pin_t * adcPin;

static bool initialized = 0;
static CS_ADC_Parameters_t adcParams = {
    .shuntResistanceMilliohms = 220000,
    .adcMaxValue = 1023,
    .referenceVoltageMillivolts = 1100
};


CS_ADC_Error_t CS_ADC_Create(CS_ADC_Parameters_t * currentSensorADC)
{
    if (currentSensorADC == NULL) return CS_ADC_ERROR_NULL_PTR;

    adcPin = BSP_GetPin(BSP_PIN_ADC0);
    if (!adcPin) return CS_ADC_ERROR_NO_PIN_ASSIGNED;

    pinMode(adcPin->pin, INPUT);
    analogReference(INTERNAL); /* Ref = 1.1V */

    *currentSensorADC = adcParams;

    initialized = 1;

    return CS_ADC_OK;
}

CS_ADC_Error_t CS_ADC_Destroy(void)
{
    initialized = false;
    return CS_ADC_OK;
}

CS_ADC_Error_t CS_ADC_GetValue(CS_ADC_Value_t * rawVoltageReading)
{
    if (!initialized) return CS_ADC_ERROR_NOT_INITIALIZED;

    if (rawVoltageReading == NULL) return CS_ADC_ERROR_NULL_PTR;

    int adcValue = analogRead(adcPin->pin); // blocking read (0..1023)

    *rawVoltageReading = (CS_ADC_Value_t)adcValue;

    return CS_ADC_OK;
}