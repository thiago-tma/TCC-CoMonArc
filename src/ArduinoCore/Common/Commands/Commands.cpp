#include <Arduino.h>
#include <Commands.h>
#include <Logger/include/log_api.h>

static void Command_Ping(int argc, char ** argv)
{
    log_command_event_ping();
}

const CommHandler_Command_t  systemCommands []
{
    {"ping",            Command_Ping},
    {"", 0}
};

const CommHandler_Command_t * Commands_GetCommandTable (void)
{
    return systemCommands;
}