#include "FakeCS_ADC.h"

static bool initialized = false;

static CS_ADC_Parameters_t storedParameters;
static CS_ADC_Value_t storedValue = 0;

CS_ADC_Error_t  CS_ADC_Create(CS_ADC_Parameters_t * currentSensorADC)
{
    initialized = true;

    currentSensorADC->adcMaxValue =storedParameters.adcMaxValue;
    currentSensorADC->referenceVoltageMillivolts = storedParameters.referenceVoltageMillivolts;
    currentSensorADC->shuntResistanceMilliohms = storedParameters.shuntResistanceMilliohms;
}

CS_ADC_Error_t  CS_ADC_Destroy(void)
{
    initialized = false;
    FakeCS_ADC_SetParameters((CS_ADC_Parameters_t){.adcMaxValue=1,.referenceVoltageMillivolts=1, .shuntResistanceMilliohms = 1});
    storedValue = 0;
}

CS_ADC_Error_t  CS_ADC_GetValue(CS_ADC_Value_t * rawVoltageReading)
{
    *rawVoltageReading = storedValue;
}

void FakeCS_ADC_Reset(void);

bool FakeCS_ADC_GetInitialized (void)
{
    return initialized;
}

void FakeCS_ADC_SetParameters (CS_ADC_Parameters_t parameters)
{
    storedParameters.adcMaxValue = parameters.adcMaxValue;
    storedParameters.referenceVoltageMillivolts = parameters.referenceVoltageMillivolts;
    storedParameters.shuntResistanceMilliohms = parameters.shuntResistanceMilliohms;
}

void FakeCS_ADC_SetRead (CS_ADC_Value_t rawValue)
{
    storedValue = rawValue;
}
