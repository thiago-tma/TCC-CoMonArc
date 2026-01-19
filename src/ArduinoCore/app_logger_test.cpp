#include <Arduino.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>

void transmitCallback (uint8_t * payload, size_t payloadSize)
{
    /* Add a byte identifying the beggining of the message */
    for (size_t index = 0; index < payloadSize; index++)
    {
        Serial.write(payload[index]);
    }   
}


void setup() 
{
    Serial.begin(9600);

    Logger_Create();

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(transmitCallback);

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
    delay(2000);
}