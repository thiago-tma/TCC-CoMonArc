#ifndef D_COMMANDHANDLERFAKE_H
#define D_COMMANDHANDLERFAKE_H

#include <CommandHandler/include/CommandHandler.h>
#include <stddef.h>

#define FAKE_COMMAND_HANLDER_MAX_BUFFER_SIZE 500

void FakeCommandHandler_Reset (void);

void FakeCommandHandler_GetSentString(char ** string, size_t * sizeofString);

#endif /*D_COMMANDHANDLERFAKE_H*/