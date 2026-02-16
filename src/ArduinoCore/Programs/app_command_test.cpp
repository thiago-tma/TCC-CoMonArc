#include <Arduino.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <HAL/UART.h>

static void UART_Transmit  (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    HAL_UART_SendByte(0xAA);
    HAL_UART_SendByte(messageID);
    HAL_UART_SendByte(payloadSize);
    if (payloadSize > 0)
    {
        HAL_UART_SendPayload(payload, payloadSize);
    }
}

static void initializeLogger (void)
{
    Logger_Create(LOGGER_MODE_MIXED);

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(UART_Transmit, TRANSMITTER_CALLBACK_GROUP_DELAYED);

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
}

void loop (void)
{
    Receiver_Run();
    Logger_Flush();
}