#include "FakeCommandHandler.h"


static char fakeCommHandlerBuffer[FAKE_COMMAND_HANLDER_MAX_BUFFER_SIZE];
static size_t characterCount = 0;
static bool initialized = true;

CommHandler_Error_t CommandHandler_Create(CommHandler_Command_t * commandTable)
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