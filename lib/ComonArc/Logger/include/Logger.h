#ifndef D_LOGGER_H
#define D_LOGGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "log_ids.h"

#define LOGGER_MAX_BUFFER_SIZE 180
#define LOGGER_BUFFER_OVERFLOW_ERROR_SPACE 10       /* Espaço no buffer reservado para registrar erro de buffer overflow */

typedef void (*Log_ErrorCallback_t)(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

/* Habilita módulo */
void Logger_Create      (void);

/* Apaga estado interno (mensagens e callback) e desabilita módulo */
void Logger_Destroy     (void);

/* Verifica nível da mensagem e armazena caso esteja habilitada */
void Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

/* Passar LOG_SUBSYSTEM_COUNT (número de subsistemas existentes) aplica o filtro para todos os subsistemas      */
/* inclusive = true aplica o estado do filtro para os outros níveis                                             */
/*      enable = true: níveis antecessores também são habilitados                                               */
/*      enable = false: níveis posteriores também são desabilitados                                             */
/* Estado inicial do filtro sempre está em todos os níveis desabilitados                                        */
/* Supõe-se que Log_Subsystem_t e Log_Level_t sejam enums com começo em zero e tenham o último elemento         */
/*      reservado para contagem dos subsistemas e níveis existentes (LOG_SUBSYSTEM_COUNT e LOG_LEVEL_COUNT)     */
void Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive);

/* Realiza a transmissão síncrona das mensagens acumuladas através do Transmitter */
void Logger_Flush (void);

/* Função chamada quando uma mensagem do tipo 'Error' é registrada (Logger_log) */
void Logger_AttachErrorCallback (Log_ErrorCallback_t errorCallback);


#endif  /*D_LOGGER_H*/