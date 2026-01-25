#include <Arduino.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>
#include <HAL/UART.h>
#include <SystemClock.h>

void setup() 
{
    //pinMode(LED_BUILTIN, OUTPUT);
    Logger_Create();

    SystemClock_Create();
    HAL_UART_Init(4800);
    Transmitter_Create();
    Transmitter_AttachTransmitCallback(HAL_UART_SendPayload);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_TRACE, true, true); /* Enable all messages */
}

void loop() 
{
    static int16_t reading = 0;

    char message[] = "This is a raw test message";
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_TRACE, (Log_MessageId_t)0, (uint8_t*)message, sizeof(message)); /* test generic*/
    log_command_trace_initialized(); /* test trace message with no arguments */
    log_logger_error_buffer_overflow();
    log_servo_data_direction(reading); /* test data message with one argument */
    Logger_Flush();
    reading++;
    delay(3000);
}