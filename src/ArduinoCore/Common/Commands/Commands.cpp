#include <Arduino.h>
#include <Commands.h>
#include <Logger/include/log_api.h>
#include <UserInterface.h>

static void Command_Ping(int argc, char ** argv)
{
    log_command_event_ping();
}

static void Command_Led(int argc, char ** argv)
{
    UserInterface_BlinkComponent(ACTUATOR_LED, 2, 50000, 50000);
}

static void Command_Buzz(int argc, char ** argv)
{
    UserInterface_BlinkComponent(ACTUATOR_BUZZER, 2, 50000, 50000);
}

const CommHandler_Command_t  systemCommands []
{
    {"ping",            Command_Ping},
    {"led",             Command_Led},
    {"buzz",            Command_Buzz},
    {"", 0}
};

const CommHandler_Command_t * Commands_GetCommandTable (void)
{
    return systemCommands;
}