#ifndef D_SOFTTIMER_H
#define D_SOFTTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "TimeTypes.h"

typedef struct SoftTimerStruct
{
    timeMicroseconds startTime;
    timeMicroseconds timerPeriod;
    bool active;
} SoftTimer;

/* Configura timer para retornar verdadeiro em [timerPeriod] microssegundos */
/* Retorna falso se [timerPeriod] == 0 */
bool        SoftTimer_Create    (SoftTimer * softTimer, timeMicroseconds timerPeriod);

/* Sem efeito, retorna true */
bool        SoftTimer_Destroy   (SoftTimer * softTimer);

/* Retorna true se timer expirou, falso caso contrário      */
/* Contagem é reiniciada assim que chamada retornar 'true'  */
/* Supõe que SystemClock foi inicializado para contar o tempo */
bool        SoftTimer_Check     (SoftTimer * softTimer);

/* Reinicia contagem */
void        SoftTimer_Reset     (SoftTimer * softTimer);

#ifdef __cplusplus
}
#endif

#endif /*D_SOFTTIMER_H*/