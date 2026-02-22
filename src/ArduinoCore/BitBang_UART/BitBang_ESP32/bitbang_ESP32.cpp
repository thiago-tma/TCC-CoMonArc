#include <Arduino.h>
#include <HAL/UART.h>

/* Use same TX pin as hardware UART0 on ESP32 */
#define TX_PIN 1   // GPIO1 (U0TXD). Change if needed.

/* Fast GPIO macros for ESP32 */
#define TX_HIGH()  GPIO.out_w1ts = (1UL << TX_PIN)
#define TX_LOW()   GPIO.out_w1tc = (1UL << TX_PIN)

static unsigned long bitTime_us = 0;
static unsigned long nextBitTime_us = 0;
static bool uartInitialized = false;

static inline void uartDelayBit(void)
{
    nextBitTime_us += bitTime_us;
    while ((nextBitTime_us - micros()) <= bitTime_us)
    {
        // busy wait
    }
}

void HAL_UART_Init(uint32_t baudrate)
{
    pinMode(TX_PIN, OUTPUT);

    /* Idle state = HIGH */
    TX_HIGH();

    bitTime_us = 1000000UL / baudrate;
    uartInitialized = true;
}

void HAL_UART_Deinit(void)
{
    uartInitialized = false;
}

/* Send one byte (8N1) */
void HAL_UART_SendByte(uint8_t byte)
{
    if (!uartInitialized) return;

    /* Start bit (LOW) */
    nextBitTime_us = micros();
    TX_LOW();
    uartDelayBit();

    /* Data bits (LSB first) */
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x01)
            TX_HIGH();
        else
            TX_LOW();

        uartDelayBit();
        byte >>= 1;
    }

    /* Stop bit (HIGH) */
    TX_HIGH();
    uartDelayBit();
}

void HAL_UART_SendPayload(uint8_t *payload, size_t payloadSize)
{
    for (size_t i = 0; i < payloadSize; i++)
    {
        HAL_UART_SendByte(payload[i]);
    }
}

void HAL_UART_ReceivePayload(size_t messageMaxSize, char *message, size_t *messageSize)
{
    /* Not implemented */
    *messageSize = 0;
}

bool HAL_UART_GetByte(uint8_t *byte)
{
    /* Not implemented */
    return false;
}