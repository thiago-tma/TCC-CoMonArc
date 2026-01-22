#include <Arduino.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>

void setup (void)
{
    CommandHandler_Create(systemCommands)
}

void loop (void)
{

}