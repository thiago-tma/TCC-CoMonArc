#include <Arduino.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <HAL/UART.h>
#include <YawController.h>
#include <Magnetometer.h>


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
    HAL_UART_Init(9600);
    CommandHandler_Create(Commands_GetCommandTable());

    initializeLogger();
    initializeReceiver();

    YawController_Create();
    Magnetometer_Create();
}

void loop (void)
{
    Magnetometer_NewRead();
    YawController_Run();
    Receiver_Run();
    Logger_Flush();
    delay(1000);
}