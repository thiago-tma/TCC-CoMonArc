#include <Arduino.h>
#include <UserInterface.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <HAL/UART.h>
#include <SystemClock.h>
#include <SoftTimer.h>

static SoftTimer errorActionTimer;

static void buttonCallback (void)
{
    UserInterface_BlinkComponent(ACTUATOR_BUZZER, 3, 50000, 50000);
}

static void errorCallback (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    /* Reset system to a safe state and loop over sending the error message */
    Logger_DetachErrorCallback(); /* Prevent a recursive action over logging the same error again */
    SoftTimer_Create(&errorActionTimer, 3000000); /* 3 seconds */
    UserInterface_BlinkComponent(ACTUATOR_BUZZER, 4, 50000, 50000);
    UserInterface_BlinkComponent(ACTUATOR_LED, 4, 50000, 50000);

    while (1)
    {
        Logger_Flush();
        UserInterface_Run();
        if (SoftTimer_Check(&errorActionTimer))
        {
            Logger_Log(origin, level, messageID, payload, payloadSize);
            UserInterface_BlinkComponent(ACTUATOR_BUZZER, 4, 50000, 50000);
            UserInterface_BlinkComponent(ACTUATOR_LED, 4, 50000, 50000);
        }
    }   
}

static void initializeLogger (void)
{
    Logger_Create();

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(HAL_UART_SendPayload);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_TRACE, true, true); /* Enable all messages */
}

static void initializeCommandHandler (void)
{
    CommandHandler_Create(Commands_GetCommandTable());
    Receiver_Create(true, HAL_UART_ReceivePayload, micros);       
}

void setup (void)
{
    HAL_UART_Init(9600);

    initializeCommandHandler();
    initializeLogger();
    Logger_AttachErrorCallback(errorCallback);

    SystemClock_Create();
    UserInterface_Create();
    UserInterface_AddButtonFunction(buttonCallback);
}

void loop (void)
{
    UserInterface_Run();
    Receiver_Run();
    Logger_Flush();
}