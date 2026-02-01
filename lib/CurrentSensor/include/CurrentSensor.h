#ifndef H_CURRENTSENSOR_D
#define H_CURRENTSENSOR_D

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int32_t Current_Microamps_t;
typedef int32_t Voltage_Millivolts_t;


typedef enum 
{
    CURRENTSENSOR_OK,
    CURRENTSENSOR_ERROR_ALREADY_INITIALIZED,
    CURRENTSENSOR_ERROR_NOT_INITIALIZED,
    CURRENTSENSOR_ERROR_NO_AVAILABLE_READING,
    CURRENTSENSOR_ERROR_ADC_NULL_PARAMETER,
}   CurrentSensor_Error_t;

CurrentSensor_Error_t CurrentSensor_Create(void);
CurrentSensor_Error_t CurrentSensor_Destroy(void);

/* Leitura síncrona do sensor de corrente */
/* Traduz leitura para os formatos utilizados pelo sistema, disponível em CurrentSensor_GetValue */
CurrentSensor_Error_t CurrentSensor_NewRead(void);

CurrentSensor_Error_t CurrentSensor_GetCurrent(Current_Microamps_t * current);
CurrentSensor_Error_t CurrentSensor_GetVoltage(Voltage_Millivolts_t * voltage);

#ifdef __cplusplus
}
#endif

#endif /*H_CURRENTSENSOR_D*/