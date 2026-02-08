#include <Logger/include/Logger.h>
#include <Logger/include/log_api.h>
#include <Transmitter/include/Transmitter.h>

static uint8_t logBuffer[LOGGER_MAX_BUFFER_SIZE]; 
static size_t logBufferIndex;
static bool logFilter[LOG_SUBSYSTEM_COUNT][LOG_LEVEL_COUNT];
static bool initialized = false;
static bool overflowFlag = false;
static Logger_Mode_t operatingMode;

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

Logger_Error_t Logger_Create      (Logger_Mode_t mode) 
{
    if (initialized) return LOGGER_ERROR_ALREADY_INITIALIZED;

    logBufferIndex = 0;
    resetFilter();
    overflowFlag = false;

    operatingMode = mode;

    Transmitter_Create();

    initialized = true;
    log_logger_trace_logger_initialized();

    return LOGGER_OK;
}

Logger_Error_t Logger_Destroy     (void) 
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;

    initialized = false;

    return LOGGER_OK;
}

Logger_Error_t Logger_ResetFilter()
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;
    resetFilter();
    return LOGGER_OK;
}

Logger_Error_t Logger_Log(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize) 
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;

    /* Filter message */
    if (logFilter[origin][level] != true) return LOGGER_ERROR_MESSAGE_FILTERED;
    
    if (operatingMode == LOGGER_MODE_INSTANT || operatingMode == LOGGER_MODE_MIXED)
    {
        Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_INSTANT, origin, level, messageID, payload, payloadSize);
    }
    
    if (operatingMode == LOGGER_MODE_BUFFERED || operatingMode == LOGGER_MODE_MIXED)
    {
        /* If not enough space available in buffer for the whole message */
        /*(LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE) - logBufferIndex < (LOGGER_MESSAGE_MIN_LENGHT + payloadSize)*/
        if ((LOGGER_MAX_BUFFER_SIZE) < (LOGGER_MESSAGE_MIN_LENGHT + payloadSize + logBufferIndex))
        {
            overflowFlag = true;
            return LOGGER_ERROR_MESSAGE_BUFFER_FULL;
        }
    
        logBuffer[logBufferIndex++] = origin;
        logBuffer[logBufferIndex++] = level;
        logBuffer[logBufferIndex++] = messageID;
        logBuffer[logBufferIndex++] = payloadSize;
    
        for (int index = 0; index < payloadSize; index++)
        {
            logBuffer[logBufferIndex++] = payload[index];
        }
    }
    
    return LOGGER_OK;
}

Logger_Error_t  setFilterInOneSubsystem(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive)
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;

    if( !inclusive)
    {
        logFilter[subsystem][level] = enable;
        return LOGGER_OK;
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

    return LOGGER_OK;
}

Logger_Error_t Logger_SetFilter(Log_Subsystem_t subsystem, Log_Level_t level, bool enable, bool inclusive) 
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;

    if (subsystem != LOG_SUBSYSTEM_COUNT)
    {
        setFilterInOneSubsystem( subsystem, level, enable, inclusive);
        return LOGGER_OK;
    }

    while(subsystem-- > 0)
    {
        setFilterInOneSubsystem( subsystem, level, enable, inclusive);
    }

    return LOGGER_OK;
}

Logger_Error_t Logger_Flush (void) 
{
    if (!initialized) return LOGGER_ERROR_NOT_INITIALIZED;

    size_t transmitIndex = 0;
    Log_Subsystem_t origin = 0;
    Log_Level_t level = 0;
    Log_MessageId_t messageID = 0;
    size_t payloadSize = 0;
    uint8_t * payload = 0;

    while (transmitIndex < logBufferIndex)
    {
        origin          =   logBuffer[transmitIndex++];
        level           =   logBuffer[transmitIndex++];
        messageID       =   logBuffer[transmitIndex++];
        payloadSize     =   logBuffer[transmitIndex++];
        payload = &logBuffer[transmitIndex];

        transmitIndex += payloadSize;
        Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_DELAYED, origin, level, messageID, payload, payloadSize);
    }

    logBufferIndex = 0;

    Logger_Error_t returnCode = LOGGER_OK;
    if (overflowFlag) returnCode = LOGGER_ERROR_MESSAGE_BUFFER_FULL;
    overflowFlag = false;

    return returnCode;
}