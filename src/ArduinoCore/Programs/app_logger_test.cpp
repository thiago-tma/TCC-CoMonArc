#include <Arduino.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>
#include <Logger/include/log_api.h>

void transmitCallback (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    /* Add a byte identifying the beggining of the message */
    Serial.write(0xAA);
    Serial.write(messageID);
    Serial.write((uint8_t)payloadSize);

    for (size_t index = 0; index < payloadSize; index++)
    {
        Serial.write(payload[index]);
    }   
}


void setup() 
{
    Serial.begin(9600);

    Logger_Create(LOGGER_MODE_MIXED);

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(transmitCallback, TRANSMITTER_CALLBACK_GROUP_DELAYED);

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
    reading++;
    delay(2000);
}