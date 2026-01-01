#ifndef D_LOGGER_H
#define D_LOGGER_H

#include <stdint.h>
#include <stddef.h>
#include "log_ids.h"


void Logger_Create      (void);
void Logger_Destroy     (void);

void Logger_Log (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);


#endif  /*D_LOGGER_H*/