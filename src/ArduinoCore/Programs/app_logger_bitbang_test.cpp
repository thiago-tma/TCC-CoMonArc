#include <Arduino.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>
#include <HAL/UART.h>
#include <SystemClock.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Receiver/include/Receiver.h>
#include <Commands.h>

void UART_Transmit (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    /* Add a byte identifying the beggining of the message */
    HAL_UART_SendByte(0xAA);
    HAL_UART_SendByte(messageID);
    HAL_UART_SendByte(payloadSize);
    if (payloadSize > 0)
    {
        HAL_UART_SendPayload(payload, payloadSize);
    }
}

void setup() 
{
    Logger_Create(LOGGER_MODE_MIXED);

    SystemClock_Create();
    HAL_UART_Init(38400);
    Transmitter_Create();
    Transmitter_AttachTransmitCallback(UART_Transmit, TRANSMITTER_CALLBACK_GROUP_DELAYED);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_TRACE, true, true); /* Enable all messages */
}

void loop() 
{
    static int16_t reading = 0;

    char message[] = "This is a raw test message";
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_TRACE, (Log_MessageId_t)0, (uint8_t*)message, sizeof(message)); /* test generic*/
    log_command_trace_command_handler_initialized(); /* test trace message with no arguments */
    log_logger_error_buffer_overflow();
    log_servo_data_direction(reading); /* test data message with one argument */
    Logger_Flush();
    //Receiver_Run();
    reading++;
    delay(2000);
}