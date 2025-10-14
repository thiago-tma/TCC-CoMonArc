#ifndef D_SOFTTIMER_H
#define D_SOFTTIMER_H

#include "TimeTypes.h"

typedef struct SoftTimerStruct * SoftTimer;

SoftTimer   TimeService_CreateTimer     (timeMicroseconds timerPeriod);
bool        TimeService_DestroyTimer    (SoftTimer softTimer);
bool        TimeService_CheckTimer      (SoftTimer softTimer);
void        TimeService_ResetTimer      (SoftTimer softTimer);

#endif /*D_SOFTTIMER_H*/