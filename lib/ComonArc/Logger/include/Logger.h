#ifndef D_LOGGER_H
#define D_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "log_ids.h"

#define LOGGER_MAX_BUFFER_SIZE 180
#define LOGGER_MESSAGE_MIN_LENGHT 4  /* Subsystem, Level, Id & messageSize */   

typedef void (*Log_ErrorCallback_t)(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

typedef enum
{
    LOGGER_OK,
    LOGGER_ERROR_NOT_INITIALIZED,
    LOGGER_ERROR_ALREADY_INITIALIZED,
    LOGGER_ERROR_MESSAGE_BUFFER_FULL,
    LOGGER_ERROR_MESSAGE_FILTERED
}   Logger_Error_t;

typedef enum
{
    LOGGER_MODE_INSTANT,        /* Mensagens são enviadas ao transmitter durante as chamadas de log */
    LOGGER_MODE_BUFFERED,       /* Mensagens são armazenadas e repassadas chamanda a função flush */
    LOGGER_MODE_MIXED           /* Modo com ambos os tipo de operação ativos */
} Logger_Mode_t;

/* Habilita módulo */
Logger_Error_t Logger_Create      (Logger_Mode_t mode);

/* Apaga estado interno (mensagens e callback) e desabilita módulo */
Logger_Error_t Logger_Destroy     (void);

/* Verifica nível da mensagem e armazena caso esteja habilitada */
/* Use messageID = 0 para passar uma string como o payload ao invés de um token */
/* Mensagens não filtradas são enviadas ao transmitter para serem executadas por callbacks imediatos */
/* Armazena mensagens para serem executadas por callbacks adiados, registrados peo transmitter */
/* Caso buffer não tenha espaço para a mensagem completa, mensagem é ignorada */
Logger_Error_t Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

/* Passar LOG_SUBSYSTEM_COUNT (número de subsistemas existentes) aplica o filtro para todos os subsistemas      */
/* inclusive = true aplica o estado do filtro para os outros níveis                                             */
/*      enable = true: níveis antecessores também são habilitados                                               */
/*      enable = false: níveis posteriores também são desabilitados                                             */
/* Estado inicial do filtro sempre está em todos os níveis desabilitados                                        */
/* Supõe-se que Log_Subsystem_t e Log_Level_t sejam enums com começo em zero e tenham o último elemento         */
/*      reservado para contagem dos subsistemas e níveis existentes (LOG_SUBSYSTEM_COUNT e LOG_LEVEL_COUNT)     */
Logger_Error_t Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive);

Logger_Error_t Logger_ResetFilter();

/* Envia as mensagens armazenadas para o Transmitter, para serem executadas por callbacks adiados */
/* Em caso de mensagens ignoradas por buffer cheio, retorna erro */
Logger_Error_t Logger_Flush (void);

#ifdef __cplusplus
}
#endif

#endif  /*D_LOGGER_H*/