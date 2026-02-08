#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>

static bool initialized = false;
static TransmitCallback_t storedCallbacksInstant[TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP];
static TransmitCallback_t storedCallbacksDelayed[TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP];

Transmitter_Error_t Transmitter_Create     (void)
{
    if (initialized) return TRANSMITTER_ERROR_INITIALIZED;
    initialized = true;

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP; i++)
    {
        storedCallbacksInstant[i] = 0;
        storedCallbacksDelayed[i] = 0;
    }
    
    log_logger_trace_transmitter_initialized();

    return TRANSMITTER_OK;
}

Transmitter_Error_t Transmitter_Destroy    (void) 
{   
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    initialized = false;
    return TRANSMITTER_OK;
}

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t callback, Transmitter_Callback_Group_t group)
{
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    TransmitCallback_t * callbackBuffer = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? storedCallbacksInstant : storedCallbacksDelayed);

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP; i++)
    {
        if (!callbackBuffer[i])
        {
            callbackBuffer[i] = callback;
            return TRANSMITTER_OK;
        }
    }
    return TRANSMITTER_ERROR_CALLBACK_CAPACITY_FULL;
}

Transmitter_Error_t Transmitter_Transmit(Transmitter_Callback_Group_t group, Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    TransmitCallback_t * callbackBuffer = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? storedCallbacksInstant : storedCallbacksDelayed);

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP; i++)
    {
        if(callbackBuffer[i]) callbackBuffer[i](origin, level, messageID, payload, payloadSize);
    }

    return TRANSMITTER_OK;
}