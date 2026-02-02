#include <Logger/include/log_api.h>
#include <Receiver/include/Receiver.h>
#include <CommandHandler/include/CommandHandler.h>

static bool initialized = false;
static bool commandCallbackOperation = false;

static char commandBuffer[RECEIVER_MAX_COMMAND_BUFFER_SIZE];
static size_t commandBufferIndex = 0;

static Receiver_Timer_Callback_t storedTimerCallback = 0;
static Receiver_Command_Callback_t storedCommandCallback = 0;
static timeMicroseconds timeoutTimer = 0;

Receiver_Error_t Receiver_Create (bool callbackOperation, Receiver_Command_Callback_t commandCallback, Receiver_Timer_Callback_t timerCallback)
{
    if (!timerCallback)
    {
        log_command_error_receiver_initialization_failed();
        return RECEIVER_ERROR_NO_TIMER_CALLBACK;
    } 

    if (callbackOperation)
    {
        if (!commandCallback)
        {
            log_command_error_receiver_initialization_failed();
            return RECEIVER_ERROR_NO_COMMAND_CALLBACK;
        } 
        storedCommandCallback = commandCallback;
        commandCallbackOperation = true;
    }

    commandBufferIndex = 0;
    storedTimerCallback = timerCallback;
    initialized = true;
    log_command_trace_receiver_initialized();

    return RECEIVER_OK;
}

Receiver_Error_t Receiver_Destroy (void)
{
    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    initialized = false;
    commandCallbackOperation = false;
    storedCommandCallback = 0;
    return RECEIVER_OK;
}

static Receiver_Error_t commHandlerErrorTranslate (CommHandler_Error_t commHandlerError)
{
    CommHandler_Error_t errorCode;

    switch (commHandlerError)
    {
    case COMMHANDLER_ERROR_NOT_INITIALIZED:
        errorCode = RECEIVER_ERROR_COMMAND_HANDLER_NOT_INITIALIZED;
        break;
    case COMMHANDLER_ERROR_NO_COMMAND_FOUND:
        errorCode = RECEIVER_ERROR_COMMAND_HANDLER_NO_COMMAND_FOUND;
        break;
    default:
        errorCode = RECEIVER_ERROR_COMMAND_HANDLER_ERROR;
        break;
    }

    log_command_error_receiver_error_code(errorCode);
    return errorCode;
}

Receiver_Error_t Receiver_Run()
{
    size_t index = 0, messageSize;
    bool fullCommandDetected = false;
    CommHandler_Error_t commHandlerError;


    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;

    /* If callback operation is active */
    if (commandCallbackOperation)
    {
        storedCommandCallback(sizeof(commandBuffer)-commandBufferIndex, &commandBuffer[commandBufferIndex], &messageSize);
        if (messageSize != 0)
        {
            timeoutTimer = storedTimerCallback();
            commandBufferIndex += messageSize;
        }
    }

    /* If buffer is not empty, check if timeout occured*/
    if (commandBufferIndex != 0)
    {
        if (storedTimerCallback() - timeoutTimer > RECEIVER_TIMEOUT_MICROSECONDS)
        {
            commandBufferIndex = 0;
            log_command_event_receiver_timeout();
            return RECEIVER_ERROR_TIMEOUT;
        }
    }

    /* Check for terminating character in buffer */
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

        /*Reset buffer, throw away other accumulated bytes too*/
        commandBufferIndex = 0;

        if (commHandlerError != COMMHANDLER_OK)
        {
            return commHandlerErrorTranslate(commHandlerError);
        }
        else log_command_event_command_executed();
            
    }
    else if (commandBufferIndex == RECEIVER_MAX_COMMAND_BUFFER_SIZE)
    {
        /*Check if buffer is full and no command is detected*/
        log_command_error_receiver_buffer_overflow();
        return RECEIVER_ERROR_BUFFER_CAPACITY_REACHED;
    }

    return RECEIVER_OK;
}

Receiver_Error_t Receiver_ReceiveMessage(const char * message, size_t messageSize, size_t * receivedBytes)
{
    size_t bufferSpaceLeft;
    Receiver_Error_t returnCode = RECEIVER_OK;
    size_t setReceivedBytes = messageSize;

    if (!initialized) return RECEIVER_ERROR_NOT_INITIALIZED;
    if (!message || !receivedBytes) return RECEIVER_ERROR_NO_PARAMETER;

    if (commandCallbackOperation) return RECEIVER_ERROR_CALLBACK_OPERATION_ACTIVE;

    /* If message is too big for space left in buffer, fill until buffer is full and return error */
    bufferSpaceLeft = RECEIVER_MAX_COMMAND_BUFFER_SIZE - commandBufferIndex;
    if (messageSize > bufferSpaceLeft)
    {
        setReceivedBytes = bufferSpaceLeft;
        returnCode = RECEIVER_ERROR_BUFFER_CAPACITY_REACHED;
    }

    /* Copy message to buffer, sendo how many bytes were written and reset timeout timer */
    for (size_t index = 0; index < setReceivedBytes; index++)
    {
        commandBuffer[commandBufferIndex++] = message[index];
    }
    timeoutTimer = storedTimerCallback();
    *receivedBytes = setReceivedBytes;

    return returnCode;
}

