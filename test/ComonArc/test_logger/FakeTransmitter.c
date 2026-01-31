#include "FakeTransmitter.h"

static bool initialized = false;
static size_t bufferWriteIndex;
static uint8_t transmissionBuffer[500];

Transmitter_Error_t Transmitter_Create     (void)
{
    initialized = true;
}

Transmitter_Error_t Transmitter_Destroy    (void)
{
    initialized = false;
}

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t channel){}

Transmitter_Error_t Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize)
{
    for (int i = 0; i < payloadSize; i++)
    {
        transmissionBuffer[bufferWriteIndex+i] = payload[i];
    }
    bufferWriteIndex += payloadSize;
}

void FakeTransmitter_Reset (void)
{
    initialized = false;
    bufferWriteIndex = 0;
}

bool FakeTransmitter_GetInitialized (void)
{
    return  initialized;
}

void FakeTransmitter_ResetTransmitBuffer (void)
{
    bufferWriteIndex = 0;
}

void FakeTransmitter_GetTransmitBuffer  (uint8_t ** buffer, size_t * bufferSize)
{
    *bufferSize = bufferWriteIndex;
    *buffer = transmissionBuffer;
}