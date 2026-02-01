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
#include <CurrentSensor.h>
#include <SoftTimer.h>

#define SYSTEM_LOOP_PERIOD_US 50000

static bool yawControllerRunning = false;
static SoftTimer loopTimer;

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

    if (Magnetometer_Create() != MAGNETOMETER_OK) while (1);

    CurrentSensor_Create();

    SoftTimer_Create(&loopTimer, SYSTEM_LOOP_PERIOD_US);
}

void loop (void)
{
    Magnetometer_NewRead();
    if (yawControllerRunning) YawController_Run();
    CurrentSensor_NewRead();
    UserInterface_Run();
    Receiver_Run();
    Logger_Flush();
    
    while(!SoftTimer_Check(&loopTimer));
}