#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>

static uint8_t logBuffer[LOGGER_MAX_BUFFER_SIZE]; 
static size_t logBufferIndex;
static bool logFilter[LOG_SUBSYSTEM_COUNT][LOG_LEVEL_COUNT];
static bool loggerEnable = false;

static void resetFilter (void)
{
    for (int subsystemIndex = 0; subsystemIndex < LOG_SUBSYSTEM_COUNT; subsystemIndex++)
    {
        for (int levelIndex = 0; levelIndex < LOG_LEVEL_COUNT; levelIndex++)
        {
            logFilter[subsystemIndex][levelIndex] = false;
        }
    }
}

void Logger_Create      (void) 
{
    logBufferIndex = 0;
    resetFilter();

    loggerEnable = true;
}

void Logger_Destroy     (void) 
{
    loggerEnable = false;
}

void Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize) 
{
    if (!loggerEnable) return;

    if (logFilter[origin][level] == true)
    {
        logBuffer[logBufferIndex++] = origin;
        logBuffer[logBufferIndex++] = level;
        logBuffer[logBufferIndex++] = messageID;

        logBuffer[logBufferIndex++] = payloadSize;

        for (int index = 0; index < payloadSize; index++)
        {

            logBuffer[logBufferIndex++] = payload[index];
        }
    }
    
}

void  setFilterInOneSubsystem(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive)
{
    if( !inclusive)
    {
        logFilter[subsystem][level] = enable;
        return;
    }
    
    /* if inclusive */
    if (enable == true)
    {
        logFilter[subsystem][level] = true;
        while (level-- > 0)
        {
            logFilter[subsystem][level] = true;
        }
    }
    else
    {
        while(level != LOG_LEVEL_COUNT)
        {
            logFilter[subsystem][level] = false;
            level++;
        }
    }
}

void Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive) 
{
    if (!loggerEnable) return;

    if (subsystem != LOG_SUBSYSTEM_COUNT)
    {
        setFilterInOneSubsystem( subsystem, level, enable, inclusive);
        return;
    }

    while(subsystem-- > 0)
    {
        setFilterInOneSubsystem( subsystem, level, enable, inclusive);
    }

}

void Logger_Flush (void) 
{
    if (!loggerEnable) return;

    Transmitter_TransmitPayload(logBuffer, logBufferIndex);
    logBufferIndex = 0;
}

void Logger_AttachErrorHandler(void (*errorCallback)(void)) 
{
    if (!loggerEnable) return;
}
