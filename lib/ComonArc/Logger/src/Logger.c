#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>

static uint8_t logBuffer[LOGGER_MAX_BUFFER_SIZE]; 
static size_t logBufferIndex;
static bool logFilter[LOG_SUBSYSTEM_COUNT][LOG_LEVEL_COUNT];
static bool loggerEnable = false;
static bool overflowFlag = false;
static Log_ErrorCallback_t storedErrorCallback = 0;

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
    overflowFlag = false;
    storedErrorCallback = 0;

    loggerEnable = true;
}

void Logger_Destroy     (void) 
{
    loggerEnable = false;
}

void Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize) 
{
    if (!loggerEnable) return;

    /* If not enough space available in buffer for the whole message */
    /*(LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE) - logBufferIndex < (LOGGER_MESSAGE_MIN_LENGHT + payloadSize)*/
    if ((LOGGER_MAX_BUFFER_SIZE) < (LOGGER_MESSAGE_MIN_LENGHT + payloadSize + logBufferIndex + LOGGER_BUFFER_OVERFLOW_ERROR_SPACE))
    {
        if (!overflowFlag)
        {
            overflowFlag = true;

            logBuffer[logBufferIndex++] = LOGGER_START_BYTE;
            logBuffer[logBufferIndex++] = LOG_SUBSYS_LOGGER;
            logBuffer[logBufferIndex++] = LOG_LEVEL_ERROR;
            logBuffer[logBufferIndex++] = LOG_LOGGER_ERROR_BUFFER_OVERFLOW;
            logBuffer[logBufferIndex++] = 0;

            if (storedErrorCallback) storedErrorCallback(origin, level, messageID, payload, payloadSize);
        }
        return;
    }

    /* Keep message */
    if (logFilter[origin][level] == true)
    {
        logBuffer[logBufferIndex++] = LOGGER_START_BYTE;
        logBuffer[logBufferIndex++] = origin;
        logBuffer[logBufferIndex++] = level;
        logBuffer[logBufferIndex++] = messageID;

        logBuffer[logBufferIndex++] = payloadSize;

        for (int index = 0; index < payloadSize; index++)
        {

            logBuffer[logBufferIndex++] = payload[index];
        }

        /* Run error callback if error message and callback available */
        if (storedErrorCallback && level == LOG_LEVEL_ERROR) storedErrorCallback(origin, level, messageID, payload, payloadSize);
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
    overflowFlag = false;
}

void Logger_AttachErrorCallback(Log_ErrorCallback_t errorCallback) 
{
    if (!loggerEnable) return;

    storedErrorCallback = errorCallback;
}
