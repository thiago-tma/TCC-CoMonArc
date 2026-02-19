#include <CommandHandler/include/CommandHandler.h>
#include <Logger/include/log_api.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool initialized = false;
static const CommHandler_Command_t * storedCommandTable;

CommHandler_Error_t CommandHandler_Create(const CommHandler_Command_t * commandTable)
{
    if (!commandTable) return COMMHANDLER_ERROR_NO_COMMAND_TABLE;
    
    storedCommandTable = commandTable;
    initialized = true;

    log_command_trace_command_handler_initialized();

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
    
static const CommHandler_Command_t * searchCommand (char * commandName)
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

char * local_strtok_r (char *str, const char *delim, char **saveptr)
{
    char *end;

    if (str == NULL) str = *saveptr;

    if (str == NULL) return NULL;

    /* Skip leading delimiters */
    str += strspn(str, delim);

    if (*str == '\0') 
    {
        *saveptr = NULL;
        return NULL;
    }

    /* Find the end of the token */
    end = str + strcspn(str, delim);

    if (*end != '\0') {
        *end = '\0';
        end++;
    }

    *saveptr = end;
    return str;
}

void parseCommandString (char * commandString, int * argc, char ** argv)
{
    char *save;
    char *token = local_strtok_r(commandString, " \n", &save);

    *argc = 0;
    while ((token) && (*argc < COMMHANDLER_MAX_ARGUMENTS_IN_COMMAND)) 
    {
        argv[(*argc)++] = token;
        token = local_strtok_r(NULL, " \n", &save);
    }
}

CommHandler_Error_t CommandHandler_ProcessCommand (char * commandString, size_t commandStringSize)
{
    (void) commandStringSize;
    CommHandler_Command_t const * receivedCommand;
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
