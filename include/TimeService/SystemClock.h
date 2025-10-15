#ifndef D_SYSTEMCLOCK_H
#define D_SYSTEMCLOCK_H

#include "TimeTypes.h"
#include <stdbool.h>

/* Inicia contagem de tempo. Chamadas subsequentes (sem chamar SystemClock_Destroy) não afetam operação */
void            SystemClock_Create      (void);

/* Para contagem do tempo e zera tempo contado*/
void            SystemClock_Destroy     (void);

/*Retorna o tempo em microsegundos*/
timeMicroseconds    SystemClock_Time        (void);

#endif /*D_SYSTEMCLOCK_H*/
