#include <Arduino.h>
#include <CommandHandler/include/CommandHandler.h>
#include <Commands.h>
#include <Receiver/include/Receiver.h>
#include <Logger/include/Logger.h>
#include <Transmitter/include/Transmitter.h>


static void transmitCallback (uint8_t * payload, size_t payloadSize)
{
    /* Add a byte identifying the beggining of the message */
    for (size_t index = 0; index < payloadSize; index++)
    {
        Serial.write(payload[index]);
    }   
}

static void receiverCallback (size_t messageMaxSize, char * message, size_t * messageSize)
{
    size_t bytesReceived = 0;
    while (Serial.available() && bytesReceived < messageMaxSize)
    {
        message[bytesReceived] = (char)Serial.read();
        bytesReceived++;
    }
    *messageSize = bytesReceived;
}

static void initializeLogger (void)
{
    Logger_Create();

    Transmitter_Create();
    Transmitter_AttachTransmitCallback(transmitCallback);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_TRACE, true, true); /* Enable all messages */
}



static void initializeReceiver (void)
{
    Receiver_Create(true, receiverCallback, micros);       
}

void setup (void)
{
    Serial.begin(9600);
    CommandHandler_Create(Commands_GetCommandTable());

    initializeLogger();
    initializeReceiver();
}

void loop (void)
{
    Receiver_Run();
    Logger_Flush();
}