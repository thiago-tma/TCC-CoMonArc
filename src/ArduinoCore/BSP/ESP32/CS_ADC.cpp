#include <Arduino.h>
#include <CS_ADC.h>
#include <BSP_Pins.h>
#include <GPIO.h>
#include <Logger/include/log_api.h>

static GPIO_Pin_t * adcPin;

static bool initialized = false;
static CS_ADC_Parameters_t adcParams;

#define ESP32_ADC_RESOLUTION_BITS   12
#define ESP32_ADC_MAX_VALUE         ((1 << ESP32_ADC_RESOLUTION_BITS) - 1)

/* ================= ADC PARAMETERS ================= */

void setADCParams(void)
{
    adcParams.shuntResistanceMilliohms = 1100;
    adcParams.adcMaxValue = ESP32_ADC_MAX_VALUE;

    /* ESP32 reference depends on attenuation:
       ADC_0db  ≈ 1.1V
       ADC_2_5db ≈ 1.5V
       ADC_6db  ≈ 2.2V
       ADC_11db ≈ 3.3V
    */
    adcParams.referenceVoltageMillivolts = 1100;  // for ADC_0db
}

/* ================= DRIVER ================= */

CS_ADC_Error_t CS_ADC_Create(CS_ADC_Parameters_t * currentSensorADC)
{
    if (currentSensorADC == NULL)
        return CS_ADC_ERROR_NULL_PTR;

    adcPin = BSP_GetPin(BSP_PIN_ADC0);
    if (!adcPin)
    {
        log_current_error_initialization_failed_adc();
        return CS_ADC_ERROR_NO_PIN_ASSIGNED;
    }

    pinMode(adcPin->pin, INPUT);

    /* ESP32 ADC setup */
    analogReadResolution(ESP32_ADC_RESOLUTION_BITS);

    /* Select attenuation for expected voltage range */
    analogSetPinAttenuation(adcPin->pin, ADC_0db);  // 0–1.1V range

    setADCParams();
    *currentSensorADC = adcParams;

    initialized = true;

    return CS_ADC_OK;
}

CS_ADC_Error_t CS_ADC_Destroy(void)
{
    initialized = false;
    return CS_ADC_OK;
}

CS_ADC_Error_t CS_ADC_GetValue(CS_ADC_Value_t * rawVoltageReading)
{
    if (!initialized)
        return CS_ADC_ERROR_NOT_INITIALIZED;

    if (rawVoltageReading == NULL)
        return CS_ADC_ERROR_NULL_PTR;

    int adcValue = analogRead(adcPin->pin); // 0 .. 4095 (12-bit)

    *rawVoltageReading = (CS_ADC_Value_t)adcValue;

    return CS_ADC_OK;
}