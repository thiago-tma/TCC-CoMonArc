#include "CircularBuffer.h"

bool CircularBuffer_Create (CircularBuffer buffer ,void * const bufferStorage, size_t elementsNumber, size_t elementSize)
{
    if (!bufferStorage || !elementsNumber || !elementSize)
    {
        return false;
    }

    return true;
}

bool CircularBuffer_Destroy (CircularBuffer buffer)
{
    return true;
}

bool CircularBuffer_IsEmpty (CircularBuffer buffer)
{
    return true;
}

bool CircularBuffer_Read (CircularBuffer buffer, void *  elementOut)
{
    return false;
}

bool CircularBuffer_Write (CircularBuffer buffer, void const  * elementIn)
{
    return true;
}