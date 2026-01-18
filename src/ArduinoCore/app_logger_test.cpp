#include <Arduino.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>

void transmitCallback (uint8_t * payload, size_t payloadSize)
{
    /* Add a byte identifying the beggining of the message */
    Serial.write((uint8_t)0xAA);
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
    Logger_SetFilter(LOG_SUBSYS_COMMAND, LOG_LEVEL_TRACE, true, false);
}

void loop() 
{
    //char message[] = "This is a raw test message\n";
    //Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_TRACE, (Log_MessageId_t)0, (uint8_t*)message, sizeof(message)); /* test generic*/
    log_logger_error_buffer_overflow();
    //log_command_trace_initialized(); /* test trace message with no arguments */
    //log_servo_data_direction(12345); /* test data message with one argument */
    Logger_Flush();
    delay(2000);
}