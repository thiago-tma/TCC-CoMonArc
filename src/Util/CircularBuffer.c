#include "CircularBuffer.h"

bool  CircularBuffer_Create   (CircularBuffer * buffer , uint8_t * bufferMemory, size_t bufferSize)
{
    if (!buffer || !bufferMemory || !bufferSize)
    {
        return false;
    }

    return true;
}