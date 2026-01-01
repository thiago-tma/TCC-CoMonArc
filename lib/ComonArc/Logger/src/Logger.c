#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>

static uint8_t logBuffer[LOGGER_MAX_BUFFER_SIZE]; 
static size_t logBufferIndex;
static bool logFilter[LOG_SUBSYSTEM_COUNT][LOG_LEVEL_COUNT];

static void resetFilter (void)
{
    for (int subsystemIndex = 0; subsystemIndex < LOG_SUBSYSTEM_COUNT; subsystemIndex++)
    {
        for (int levelIndex = 0; levelIndex < LOG_LEVEL_COUNT; levelIndex++)
        {
            if (levelIndex == LOG_LEVEL_ERROR)
            {
                logFilter[subsystemIndex][LOG_LEVEL_ERROR] = true;
            }
            else logFilter[subsystemIndex][levelIndex] = false;
        }
    }
}

void Logger_Create      (void) 
{
    logBufferIndex = 0;
    resetFilter();
}

void Logger_Destroy     (void) {}

void Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize) 
{
    if (level == LOG_LEVEL_ERROR)
    {
        logBuffer[logBufferIndex++] = origin;
        logBuffer[logBufferIndex++] = level;
        logBuffer[logBufferIndex++] = messageID;
    }
    
}

void Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable) {}

void Logger_Flush (void) 
{
    Transmitter_TransmitPayload(logBuffer, logBufferIndex);
}

void Logger_AttachErrorHandler(void (*errorCallback)(void)) {}
