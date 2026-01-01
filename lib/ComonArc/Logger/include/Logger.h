#ifndef D_LOGGER_H
#define D_LOGGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "log_ids.h"

#define LOGGER_MAX_BUFFER_SIZE 180
#define LOGGER_BUFFER_OVERFLOW_ERROR_SPACE 10       /* Espaço no buffer reservado para registrar erro de buffer overflow */

/* Habilita módulo */
void Logger_Create      (void);

/* Apaga estado interno (mensagens e callback) e desabilita módulo */
void Logger_Destroy     (void);

/* Verifica nível da mensagem e armazena caso esteja habilitada */
void Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

/* Estado inicial do filtro sempre está em todos os níveis desabilitados, exceto 'Error' */
/* Filtro tem o nível 'Error' sempre habilitado */
void Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable);

/* Realiza a transmissão síncrona das mensagens acumuladas através do Transmitter */
void Logger_Flush (void);

/* Função chamada quando uma mensagem do tipo 'Error' é registrada (Logger_log) */
void Logger_AttachErrorHandler(void (*errorCallback)(void));

#endif  /*D_LOGGER_H*/