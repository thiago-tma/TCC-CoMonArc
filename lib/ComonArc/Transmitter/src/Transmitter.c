#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>

static bool initialized = false;
static TransmitCallback_t storedCallbacks[TRANSMITTER_MAX_ATTACHED_CALLBACKS];

Transmitter_Error_t Transmitter_Create     (void)
{
    if (initialized) return TRANSMITTER_ERROR_INITIALIZED;
    initialized = true;

    log_logger_trace_transmitter_initialized();

    return TRANSMITTER_OK;
}

Transmitter_Error_t Transmitter_Destroy    (void) 
{   
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        storedCallbacks[i] = 0;
    }

    initialized = false;
    return TRANSMITTER_OK;
}

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t channel)
{
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        if (!storedCallbacks[i])
        {
            storedCallbacks[i] = channel;
            return TRANSMITTER_OK;
        }
    }
    return TRANSMITTER_ERROR_CALLBACK_CAPACITY_FULL;
}

Transmitter_Error_t Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize)
{
    if (!initialized) return TRANSMITTER_ERROR_NOT_INITIALIZED;

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        if(storedCallbacks[i]) storedCallbacks[i](payload, payloadSize);
    }

    return TRANSMITTER_OK;
}