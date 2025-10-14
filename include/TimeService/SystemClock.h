#ifndef D_SYSTEMCLOCK_H
#define D_SYSTEMCLOCK_H

#include "TimeTypes.h"
#include <stdbool.h>

void            SystemClock_Create      (void);
void            SystemClock_Destroy     (void);

/*Retorna o tempo em microsegundos*/
timeMicroseconds    SystemClock_Time        (void);

#endif /*D_SYSTEMCLOCK_H*/
