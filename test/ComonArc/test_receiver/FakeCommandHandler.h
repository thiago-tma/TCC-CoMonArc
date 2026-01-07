#ifndef D_COMMANDHANDLERFAKE_H
#define D_COMMANDHANDLERFAKE_H

#include <CommandHandler/include/CommandHandler.h>
#include <stddef.h>
#include <stdbool.h>

#define FAKE_COMMAND_HANLDER_MAX_BUFFER_SIZE 500

void FakeCommandHandler_Reset (void);

void FakeCommandHandler_GetSentString(char ** string, size_t * sizeofString);
void FakeCommandHandler_SetInitialized(bool state);

#endif /*D_COMMANDHANDLERFAKE_H*/