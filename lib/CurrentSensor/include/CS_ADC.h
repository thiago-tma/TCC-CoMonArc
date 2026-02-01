#ifndef H_CS_ADC_D
#define H_CS_ADC_D

#ifdef __cplusplus
extern "C" {
#endif


/* Dependência do sensor de corrente (CurrentSensor) */
/* Supõe o uso da medida de tensão de um resistor pequeno para obter a corrente desejada */
/* Implementação é parte da HAL ou usa a HAL */

#include <stdint.h>

typedef int32_t CS_ADC_Value_t;

typedef struct 
{
    int32_t    shuntResistanceMilliohms;
    int32_t    adcMaxValue;
    int32_t    referenceVoltageMillivolts;
} CS_ADC_Parameters_t;

typedef enum 
{
    CS_ADC_OK,
    CS_ADC_ERROR_NULL_PTR,
    CS_ADC_ERROR_NOT_INITIALIZED,
    CS_ADC_ERROR_NO_PIN_ASSIGNED
}   CS_ADC_Error_t;

/* Função passa parâmetros do adc sendo usado para medir corrente por resistência aterrada */
CS_ADC_Error_t  CS_ADC_Create(CS_ADC_Parameters_t * currentSensorADC);
CS_ADC_Error_t  CS_ADC_Destroy(void);

/* Leitura síncrona do ADC */
CS_ADC_Error_t  CS_ADC_GetValue(CS_ADC_Value_t * rawVoltageReading);

#ifdef __cplusplus
}
#endif

#endif /*H_CS_ADC_D*/