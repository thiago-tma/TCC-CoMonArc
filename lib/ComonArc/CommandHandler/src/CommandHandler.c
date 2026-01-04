#include <CommandHandler/include/CommandHandler.h>
#include <stdbool.h>
#include <string.h>

static bool initialized = false;
static CommHandler_Command_t * storedCommandTable;

CommHandler_Error_t CommandHandler_Create(CommHandler_Command_t * commandTable)
{
    if (!commandTable) return COMMHANDLER_ERROR_NO_COMMAND_TABLE;
    
    storedCommandTable = commandTable;
    initialized = true;

    return COMMHANDLER_OK;
}

CommHandler_Error_t CommandHandler_Destroy(void)
{
    if (initialized)
    {
        initialized = false;
        return COMMHANDLER_OK;
    }

    return COMMHANDLER_ERROR_NOT_INITIALIZED;
}
    
static CommHandler_Command_t * searchCommand (char * commandName)
{
    unsigned int index = 0;
    while (storedCommandTable[index].function)
    {
        /* Se strings são equivalentes */
        if (strcmp(storedCommandTable[index].name, commandName) == 0)
        {
            return &storedCommandTable[index];
        }
        index++;
    }

    return 0;
}

void parseCommandString (char * commandString, int * argc, char ** argv)
{
    char *save;
    char *token = strtok_r(commandString, " \n", &save);

    *argc = 0;
    while ((token) && (*argc < COMMHANDLER_MAX_ARGUMENTS_IN_COMMAND)) 
    {
        argv[(*argc)++] = token;
        token = strtok_r(NULL, " \n", &save);
    }
}

CommHandler_Error_t CommandHandler_ProcessCommand (char * commandString, size_t commandStringSize)
{
    CommHandler_Command_t * receivedCommand;
    int argc = 0;
    char * argv[COMMHANDLER_MAX_ARGUMENTS_IN_COMMAND];

    if (!initialized) return COMMHANDLER_ERROR_NOT_INITIALIZED;
    if (*commandString == '\0' || !commandString) return COMMHANDLER_ERROR_NO_STRING;

    parseCommandString(commandString, &argc, argv);
    receivedCommand = searchCommand(argv[0]);

    if (!receivedCommand) return COMMHANDLER_ERROR_NO_COMMAND_FOUND;

    receivedCommand->function(argc, argv);

    return COMMHANDLER_OK;
}
