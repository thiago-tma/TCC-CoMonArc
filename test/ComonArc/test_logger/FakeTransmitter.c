#include "FakeTransmitter.h"

static size_t bufferWriteIndex;
static uint8_t transmissionBuffer[100];

void Transmitter_Create     (void){}
void Transmitter_Destroy    (void){}

bool Transmitter_AttachTransmitCallback(TransmitCallback_t channel){}

void Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize)
{
    for (int i = 0; i < payloadSize; i++)
    {
        transmissionBuffer[bufferWriteIndex+i] = payload[i];
    }
    bufferWriteIndex += payloadSize;
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