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
    buffer->count = 0;

    return true;
}

void CircularBuffer_Destroy(CircularBuffer * buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
    buffer->count = 0;
}

bool CircularBuffer_IsEmpty (CircularBuffer * buffer)
{
    if (buffer->count == 0)
    {
        return true;
    }

    return false;
}

bool CircularBuffer_IsFull (CircularBuffer * buffer)
{
    if (buffer->count == buffer->bufferSize)
    {
        return true;
    }

    return false;
}

size_t CircularBuffer_FreeSpace (CircularBuffer * buffer)
{
    return (buffer->bufferSize - buffer->count);
}

bool CircularBuffer_ReadOne (CircularBuffer * buffer, uint8_t * elementOut)
{
    if (!CircularBuffer_IsEmpty(buffer))
    {
        *elementOut = buffer->buffer[buffer->tail];
        buffer->tail++;
        if (buffer->tail == buffer->bufferSize)
        {
            buffer->tail = 0;
        }
        buffer->count--;

        return true;
    }

    return false;
}

bool CircularBuffer_WriteOne (CircularBuffer * buffer, uint8_t elementIn)
{
    if (CircularBuffer_IsFull(buffer))
    {
        return false;
    }

    buffer->buffer[buffer->head] = elementIn;
    buffer->head++;
    if (buffer->head == buffer->bufferSize)
    {
        buffer->head = 0;
    }
    buffer->count++;

    return true;
}

size_t CircularBuffer_WriteMany (CircularBuffer * buffer, uint8_t * elementsIn, size_t lenght)
{
    size_t i = 0;
    for (; i < lenght; i++)
    {
        if (!CircularBuffer_WriteOne(buffer, elementsIn[i]))
        {
            break;
        }
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