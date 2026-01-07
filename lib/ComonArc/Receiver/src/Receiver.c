#include <Receiver/include/Receiver.h>
#include <CommandHandler/include/CommandHandler.h>

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

static Receiver_Error_t commHandlerErrorTranslate (CommHandler_Error_t commHandlerError)
{
    switch (commHandlerError)
    {
    case COMMHANDLER_ERROR_NOT_INITIALIZED:
        return RECEIVER_ERROR_COMMANDHANDLER_NOT_INITIALIZED;
        break;
    case COMMHANDLER_ERROR_NO_COMMAND_FOUND:
        return RECEIVER_ERROR_NO_COMMAND_FOUND;
        break;
    default:
        return RECEIVER_ERROR_COMMAND_HANDLER_ERROR;
        break;
    }
}

Receiver_Error_t Receiver_Run()
{
    size_t index = 0;
    bool fullCommandDetected = false;
    CommHandler_Error_t commHandlerError;

    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    for (index = 0; index < commandBufferIndex; index++)
    {
        if (commandBuffer[index] == RECEIVER_COMMAND_TERMINATING_CHARACTER)
        {
            fullCommandDetected = true;
            break;
        }
    }

    if (fullCommandDetected)
    {
        index++;
        commHandlerError = CommandHandler_ProcessCommand(commandBuffer, index);
        
        if (commHandlerError != COMMHANDLER_OK)
        {
            return commHandlerErrorTranslate(commHandlerError);
        }
            
    }

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

