#ifndef D_COMONARCCOMMANDHANDLER_H
#define D_COMONARCCOMMANDHANDLER_H

#include <stdint.h>
#include <stddef.h>

#define COMMHANDLER_MAX_ARGUMENTS_IN_COMMAND 5

typedef enum
{
    COMMHANDLER_OK = 0,
    COMMHANDLER_ERROR_NO_COMMAND_TABLE,
    COMMHANDLER_ERROR_NOT_INITIALIZED,
    COMMHANDLER_ERROR_NO_COMMAND_FOUND,
    COMMHANDLER_ERROR_NO_STRING
} CommHandler_Error_t;

typedef void (*CommHandler_CommandFunction_t)(int argc, char **argv);

typedef struct
{
    const char * name;
    CommHandler_CommandFunction_t function;

} CommHandler_Command_t;

/* Último comando de command table deve ter nome vazio ("") para identificar fim da lista */
CommHandler_Error_t CommandHandler_Create(CommHandler_Command_t * commandTable);
CommHandler_Error_t CommandHandler_Destroy(void);

/* Recebe uma string terminada com \n */
/* Os argumentos do comando devem estar separados por espaços ' ' */
CommHandler_Error_t CommandHandler_ProcessCommand (char * commandString, size_t commandStringSize);

#endif /*D_COMONARCCOMMANDHANDLER_H*/