#include "CircularBuffer.h"

bool  CircularBuffer_Create   (CircularBuffer * buffer , uint8_t * bufferMemory, size_t bufferSize)
{
    if (!buffer || !bufferMemory || !bufferSize)
    {
        return false;
    }

    buffer->buffer = bufferMemory;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->bufferSize = bufferSize;

    return true;
}

void CircularBuffer_Destroy(CircularBuffer * buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
}

bool CircularBuffer_IsEmpty (CircularBuffer * buffer)
{
    if (buffer->head == buffer->tail)
    {
        return true;
    }

    return false;
}

size_t CircularBuffer_FreeSpace (CircularBuffer * buffer)
{
    return (buffer->bufferSize - (buffer->head - buffer->tail));
}

bool CircularBuffer_ReadOne (CircularBuffer * buffer, uint8_t * elementOut)
{
    if (!CircularBuffer_IsEmpty(buffer))
    {
        *elementOut = buffer->buffer[buffer->tail];
        buffer->tail++;

        return true;
    }

    return false;
}

bool CircularBuffer_WriteOne (CircularBuffer * buffer, uint8_t elementIn)
{
    buffer->buffer[buffer->head] = elementIn;
    buffer->head++;

    return true;
}

size_t CircularBuffer_WriteMany (CircularBuffer * buffer, uint8_t * elementsIn, size_t lenght)
{
    size_t i = 0;
    for (; i < lenght; i++)
    {
        CircularBuffer_WriteOne(buffer, elementsIn[i]);
    }

    return i;
}

size_t CircularBuffer_ReadMany (CircularBuffer * buffer, uint8_t * elementsOut, size_t lenght)
{
    size_t i = 0;
    for (; i < lenght; i++)
    {
        if(!CircularBuffer_ReadOne(buffer, &elementsOut[i]))
        {
            break;
        }
    }

    return i;
}