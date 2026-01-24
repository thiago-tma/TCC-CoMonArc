#include <Arduino.h>
#include <HAL/UART.h>

void HAL_UART_Init (uint32_t baudrate)
{
    Serial.begin(baudrate);
}

void HAL_UART_Deinit (void){}

void HAL_UART_SendPayload (uint8_t * payload, size_t payloadSize)
{
    for (size_t index = 0; index < payloadSize; index++)
    {
        Serial.write(payload[index]);
    }   
}

void HAL_UART_ReceivePayload (size_t messageMaxSize, char * message, size_t * messageSize)
{
    size_t bytesReceived = 0;
    while (Serial.available() && bytesReceived < messageMaxSize)
    {
        message[bytesReceived] = (char)Serial.read();
        bytesReceived++;
    }
    *messageSize = bytesReceived;
}