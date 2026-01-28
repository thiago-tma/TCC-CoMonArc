#include <Arduino.h>
#include <Commands.h>
#include <Logger/include/log_api.h>
#include <UserInterface.h>
#include <ServoController.h>

//================================================================
//    Helper Functions
//================================================================


uint32_t parseUInt(const char * string) {
    uint32_t value = 0;
    while (*string >= '0' && *string <= '9') {
        value = value * 10 + (*string - '0');
        string++;
    }
    return value;
}

//================================================================
//    Commands
//================================================================

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

static void Command_Servo (int argc, char ** argv)
{
    uint16_t servoHeading;
    if (argc != 2) return;

    servoHeading = (uint16_t)parseUInt(argv[1]);

    ServoController_Create();
    ServoController_SetServo(servoHeading);
}

//================================================================
//    Command Table
//================================================================

const CommHandler_Command_t  systemCommands []
{
    {"ping",            Command_Ping},
    {"led",             Command_Led},
    {"buzz",            Command_Buzz},
    {"servo",           Command_Servo},
    {"", 0}
};

const CommHandler_Command_t * Commands_GetCommandTable (void)
{
    return systemCommands;
}