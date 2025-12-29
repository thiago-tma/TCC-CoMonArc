#ifndef D_SYSTEMCLOCK_H
#define D_SYSTEMCLOCK_H

#include "TimeTypes.h"
#include <stdbool.h>

#define MAX_REGISTERED_CALLBACKS 3

typedef void (*callBack) (void);

/* Inicia contagem de tempo. Chamadas subsequentes (sem chamar SystemClock_Destroy) não afetam operação */
void                SystemClock_Create      (void);

/* Para contagem do tempo e zera tempo contado*/
void                SystemClock_Destroy     (void);

/* Retorna o tempo em microsegundos*/
timeMicroseconds    SystemClock_Time        (void);

/* Registra uma função para ser chamada a cada tique do sistema */
/* Retorna o número de identificação da função, -1 se não há espaço para mais funções, -2 se função passada for nula   */
/* Não reentrante */
int                 SystemClock_AddCallback (callBack function);

/* Remove função do registro */
/* Retorna true se uma função foi encontrada e removida */
/* Retorna false se valor não corresponde a um slot ou slot está vazio */
bool                 SystemClock_RemoveCallback (int functionId);

#endif /*D_SYSTEMCLOCK_H*/
