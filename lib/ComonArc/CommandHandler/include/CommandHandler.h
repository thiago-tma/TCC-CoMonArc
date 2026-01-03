#ifndef D_COMONARCCOMMANDHANDLER_H
#define D_COMONARCCOMMANDHANDLER_H

#include <stdint.h>

typedef void (*CommHandler_CommandFuntion_t)(char const * charStream);

typedef struct
{
    CommHandler_CommandFuntion_t command;
    char const * const commandString;
    uint8_t parameterSize;
} CommHandler_Command_t;

void CommandHandler_Create(CommHandler_Command_t * commandTable);
void CommandHandler_Destroy(void);

/* Busca o comando compatível. 
/* O começo de receivedString deve ser igual a string de um dos comandos armazenados */
/* Para comandos com parâmetros, o restante deve ter o número de bytes equivalente ao número de bytes de parâmetro */
CommHandler_Command_t * CommandHandler_FindCommand (char * receivedString, uint8_t commandStringSize);

void CommandHandler_ExecuteCommand(CommHandler_Command_t * command, char * receivedString);

#endif /*D_COMONARCCOMMANDHANDLER_H*/