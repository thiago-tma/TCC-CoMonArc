#ifndef D_SYSTEMTIMER_H
#define D_SYSTEMTIMER_H

#include "TimeTypes.h"

typedef void (*Callback)(timeMicroseconds useconds);

/* timerCallback é uma função que deve ser chamada a cada período de tempo por uma interrrupção */
/* o argumento de timerCallback é o período de tempo em microsegundos */

/*  Inicia timer. Chamadas subsequentes não afetam operação  */
void SystemTimer_Create(Callback timerCallback);

/*  Para timer. */
void SystemTimer_Destroy(void);

/* Pause e Continue param e continuam as interrupções,*/
/* para que a leitura do tempo seja feita sem erros de concorrência */
void SystemTimer_Pause(void);
void SystemTimer_Continue(void);

#endif /*D_SYSTEMTIMER_H*/
