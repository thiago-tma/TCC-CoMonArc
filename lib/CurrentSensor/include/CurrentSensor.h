#ifndef H_CURRENTSENSOR_D
#define H_CURRENTSENSOR_D

#include <stdint.h>

typedef int32_t currentMicroamps;

void CurrentSensor_Create();
void CurrentSensor_Destroy();

/* Leitura assíncrona do sensor de corrente */
/* Traduz leitura para os formatos utilizados pelo sistema */
void CurrentSensor_NewRead();

/*  */
void CurrentSensor_GetValue(currentMicroamps * current);

#endif /*H_CURRENTSENSOR_D*/