#include <CurrentSensor.h>
#include <stdbool.h>
#include "CS_ADC.h"

static bool initialized = false;
static CS_ADC_Parameters_t adcParameters;
static int64_t conversionFactor = 0;

static Current_Microamps_t currentReading;
/*static Voltage_Millivolts_t voltageReading;*/
static bool readAvailable = false;

static CurrentSensor_Error_t setConversionFactor (void)
{
    if (!adcParameters.adcMaxValue || !adcParameters.referenceVoltageMillivolts || !adcParameters.shuntResistanceMilliohms)
    {
        return CURRENTSENSOR_ERROR_ADC_NULL_PARAMETER;
    }

    /*Fixed-point conversion of (referece(mV)/resolution)/resistance(mOhm) = current(uA) */
    float numerator = (float)adcParameters.referenceVoltageMillivolts*1000000;
    float denominator = (float)adcParameters.adcMaxValue*adcParameters.shuntResistanceMilliohms;
    conversionFactor = (int64_t)(numerator*1000/denominator);
    return CURRENTSENSOR_OK;
}

CurrentSensor_Error_t CurrentSensor_Create(void)
{
    CurrentSensor_Error_t error;
    if (initialized) return CURRENTSENSOR_ERROR_ALREADY_INITIALIZED;
    
    CS_ADC_Create(&adcParameters);
    readAvailable = false;
    error = setConversionFactor();
    if( error != CURRENTSENSOR_OK) return error;

    initialized = true;
    return CURRENTSENSOR_OK;
}

CurrentSensor_Error_t CurrentSensor_Destroy(void)
{
    if (!initialized) return CURRENTSENSOR_ERROR_NOT_INITIALIZED;

    CS_ADC_Destroy();
    conversionFactor = 0;

    initialized = false;
    return CURRENTSENSOR_OK;
}

CurrentSensor_Error_t CurrentSensor_NewRead(void)
{
    CS_ADC_Value_t rawValue;

    if (!initialized) return CURRENTSENSOR_ERROR_NOT_INITIALIZED;

    CS_ADC_GetValue(&rawValue);
    currentReading = (Current_Microamps_t)(rawValue*conversionFactor/1000);

    readAvailable = true;
    return CURRENTSENSOR_OK;
}

CurrentSensor_Error_t CurrentSensor_GetCurrent(Current_Microamps_t * current)
{
    if (!initialized) return CURRENTSENSOR_ERROR_NOT_INITIALIZED;

    if (!readAvailable) return CURRENTSENSOR_ERROR_NO_AVAILABLE_READING;

    *current = currentReading;

    return CURRENTSENSOR_OK;
}