#include "FakeCommandHandler.h"


static char fakeCommHandlerBuffer[FAKE_COMMAND_HANLDER_MAX_BUFFER_SIZE];
static size_t characterCount = 0;
static bool initialized = true;
static CommHandler_Error_t storedError = COMMHANDLER_OK;

CommHandler_Error_t CommandHandler_Create(const CommHandler_Command_t * commandTable)
{
    return COMMHANDLER_OK;
}

CommHandler_Error_t CommandHandler_Destroy(void)
{
    return COMMHANDLER_OK;
}


CommHandler_Error_t CommandHandler_ProcessCommand (char * commandString, size_t commandStringSize)
{
    if (!initialized) return COMMHANDLER_ERROR_NOT_INITIALIZED;
    if (storedError != COMMHANDLER_OK) return storedError;

    for (size_t index = 0; index < commandStringSize; index++)
    {
        fakeCommHandlerBuffer[characterCount++] = commandString[index];
    }

    return COMMHANDLER_OK;
}

void FakeCommandHandler_Reset (void)
{
    characterCount = 0;
    initialized = true;
    storedError = COMMHANDLER_OK;
}

void FakeCommandHandler_SetInitialized(bool state)
{
    initialized = state;
}

void FakeCommandHandler_GetSentString(char ** string, size_t * sizeofString)
{
    *string = fakeCommHandlerBuffer;
    *sizeofString = characterCount;
}

void FakeCommandHandler_SetError(CommHandler_Error_t error)
{
    storedError = error;
}