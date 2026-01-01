#include <Transmitter/include/Transmitter.h>

static TransmitCallback_t storedCallbacks[TRANSMITTER_MAX_ATTACHED_CALLBACKS];

void Transmitter_Create     (void) {}

void Transmitter_Destroy    (void) 
{
    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        storedCallbacks[i] = 0;
    }
}

bool Transmitter_AttachTransmitCallback(TransmitCallback_t channel)
{
    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        if (!storedCallbacks[i])
        {
            storedCallbacks[i] = channel;
            return true;
        }
    }
    return false;
}

void Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize)
{
    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        if(storedCallbacks[i]) storedCallbacks[i](payload, payloadSize);
    }
}