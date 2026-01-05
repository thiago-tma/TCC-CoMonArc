#include <Receiver/include/Receiver.h>

static bool initialized = false;
static char commandBuffer[RECEIVER_MAX_COMMAND_BUFFER_SIZE];
static size_t commandBufferIndex = 0;

Receiver_Error_t Receiver_Create (bool callbackOperation, Receiver_Command_Callback_t commandCallback, Receiver_Timer_Callback_t timerCallback)
{
    if (!timerCallback) return RECEIVER_ERROR_NO_TIMER_CALLBACK;
    if (callbackOperation)
    {
        if (!commandCallback) return RECEIVER_ERROR_NO_COMMAND_CALLBACK;
    }

    commandBufferIndex = 0;
    initialized = true;
    return RECEIVER_OK;
}

Receiver_Error_t Receiver_Destroy (void)
{
    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    initialized = false;
    return RECEIVER_OK;
}

Receiver_Error_t Receiver_Run()
{
    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    return RECEIVER_OK;
}

Receiver_Error_t Receiver_ReceiveMessage(const char * message, size_t messageSize, size_t * receivedBytes)
{
    size_t bufferSpaceLeft;
    Receiver_Error_t returnCode = RECEIVER_OK;
    size_t setReceivedBytes = messageSize;

    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    bufferSpaceLeft = RECEIVER_MAX_COMMAND_BUFFER_SIZE - commandBufferIndex;
    if (messageSize > bufferSpaceLeft)
    {
        setReceivedBytes = bufferSpaceLeft;
        returnCode = RECEIVER_ERROR_BUFFER_CAPACITY_REACHED;
    }

    for (size_t index = 0; index < setReceivedBytes; index++)
    {
        commandBuffer[commandBufferIndex++] = message[index];
    }

    *receivedBytes = setReceivedBytes;

    return returnCode;
}

