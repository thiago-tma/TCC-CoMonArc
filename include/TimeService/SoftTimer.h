#ifndef D_SOFTTIMER_H
#define D_SOFTTIMER_H

#include "TimeTypes.h"

typedef struct SoftTimerStruct * SoftTimer;

SoftTimer   SoftTimer_Create     (timeMicroseconds timerPeriod);
bool        SoftTimer_Destroy    (SoftTimer softTimer);
bool        SoftTimer_Check    (SoftTimer softTimer);
void        SoftTimer_Reset    (SoftTimer softTimer);

#endif /*D_SOFTTIMER_H*/