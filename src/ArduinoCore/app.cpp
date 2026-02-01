#include <Arduino.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <HAL/UART.h>
#include <YawController.h>
#include <Magnetometer.h>
#include <UserInterface.h>

static bool yawControllerRunning = false;

static void buttonFunction (void)
{
    if (yawControllerRunning)
    {
        YawController_Destroy();
        yawControllerRunning = false;
    }
    else
    {
        YawController_Create();
        yawControllerRunning = true;
    }
}

static void initializeLogger (void)
{
    Logger_Create();

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(HAL_UART_SendPayload);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_TRACE, true, true); /* Enable all messages */
}

static void initializeReceiver (void)
{
    Receiver_Create(true, HAL_UART_ReceivePayload, micros);       
}

void setup (void)
{
    UserInterface_Create();
    UserInterface_AddButtonFunction(buttonFunction);

    HAL_UART_Init(115200);
    CommandHandler_Create(Commands_GetCommandTable());

    initializeLogger();
    initializeReceiver();

    if (Magnetometer_Create() != MAGNETOMETER_OK)
    {
        pinMode(LED_BUILTIN, OUTPUT);
        while(1)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
        }
    }
}

void loop (void)
{
    Magnetometer_NewRead();
    if (yawControllerRunning) YawController_Run();
    UserInterface_Run();
    Receiver_Run();
    Logger_Flush();
    delay(50);
}