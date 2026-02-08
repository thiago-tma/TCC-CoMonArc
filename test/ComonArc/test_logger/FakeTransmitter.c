#include "FakeTransmitter.h"

static bool initialized = false;
static size_t bufferIndexInstant, bufferIndexDelayed;
static uint8_t bufferInstant[500];
static uint8_t bufferDelayed[500];

Transmitter_Error_t Transmitter_Create     (void)
{
    initialized = true;
}

Transmitter_Error_t Transmitter_Destroy    (void)
{
    initialized = false;
}

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t callback, Transmitter_Callback_Group_t group){}

Transmitter_Error_t Transmitter_Transmit (Transmitter_Callback_Group_t group , Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    uint8_t * buffer = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? bufferInstant : bufferDelayed);
    size_t * bufferIndex = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? &bufferIndexInstant : &bufferIndexDelayed);

    buffer[(*bufferIndex)++] = origin;
    buffer[(*bufferIndex)++] = level;
    buffer[(*bufferIndex)++] = messageID;

    size_t payloadIndex = 0;
    while(payloadIndex < payloadSize)
    {
        buffer[(*bufferIndex)++] = payload[payloadIndex];
        payloadIndex++;
    }
    buffer[(*bufferIndex)++] = payloadSize;
}

void FakeTransmitter_Reset (void)
{
    initialized = false;
    bufferIndexInstant = 0;
    bufferIndexDelayed = 0;
}

bool FakeTransmitter_GetInitialized (void)
{
    return  initialized;
}

void FakeTransmitter_ResetTransmitBuffer (void)
{
    bufferIndexInstant = 0;
    bufferIndexDelayed = 0;
}

void FakeTransmitter_GetTransmitBuffer  (Transmitter_Callback_Group_t group ,uint8_t ** buffer, size_t * bufferSize)
{
    *buffer = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? bufferInstant : bufferDelayed);
    *bufferSize = (group == TRANSMITTER_CALLBACK_GROUP_INSTANT ? bufferIndexInstant : bufferIndexDelayed);
}