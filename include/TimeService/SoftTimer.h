#ifndef D_SOFTTIMER_H
#define D_SOFTTIMER_H

#include <stdbool.h>
#include "TimeTypes.h"

typedef struct SoftTimerStruct
{
    timeMicroseconds startTime;
    timeMicroseconds timerPeriod;
} SoftTimer;

/* Configura timer para retornar verdadeiro em [timerPeriod] microssegundos */
/* Retorna falso se [timerPeriod] == 0 */
bool        SoftTimer_Create    (SoftTimer * softTimer, timeMicroseconds timerPeriod);

/* Sem efeito, retorna true */
bool        SoftTimer_Destroy   (SoftTimer * softTimer);

/* Retorna true se timer expirou, falso caso contrário */
/* */
bool        SoftTimer_Check     (SoftTimer * softTimer);

void        SoftTimer_Reset     (SoftTimer * softTimer);

#endif /*D_SOFTTIMER_H*/