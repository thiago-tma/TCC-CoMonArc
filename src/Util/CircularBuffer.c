#include "CircularBuffer.h"

typedef struct CircularBufferStruct
{
    void * memory;
    size_t elementsNumber;
    size_t elementSize;
    bool written;

} CircularBufferStruct;

static CircularBufferStruct storedBuffer;

bool CircularBuffer_Create (CircularBuffer * buffer ,void * const bufferMemory, size_t elementsNumber, size_t elementSize)
{
    if (!bufferMemory || !elementsNumber || !elementSize)
    {
        return false;
    }

    storedBuffer.memory =           bufferMemory;
    storedBuffer.elementSize =      elementSize;
    storedBuffer.elementsNumber =   elementsNumber;

    storedBuffer.written = false;

    *(buffer) = &storedBuffer;

    return true;
}

bool CircularBuffer_Destroy (CircularBuffer buffer)
{
    return true;
}

bool CircularBuffer_IsEmpty (CircularBuffer buffer)
{
    return (!buffer->written);
}

bool CircularBuffer_Read (CircularBuffer buffer, void *  elementOut)
{
    unsigned char * memoryCursor, * elementCursor;

    if (buffer->written)
    {
        memoryCursor = (unsigned char *) buffer->memory;
        elementCursor = (unsigned char *)elementOut;
        *elementCursor = memoryCursor[0];

        return true;
    }
    return false;
}

bool CircularBuffer_Write (CircularBuffer buffer, void const  * elementIn)
{
    unsigned char * memoryCursor, *elementCursor;

    memoryCursor = (unsigned char *)buffer->memory;
    elementCursor = (unsigned char *)elementIn;
    memoryCursor[0] = *elementCursor;

    buffer->written = true;
    return true;
}