#include <Arduino.h>
#include <HAL/UART.h>

/* Use same TX pin as hardware UART: Arduino Uno D1 = PD1 */
#define TX_PORT PORTD
#define TX_DDR  DDRD
#define TX_BIT  1   // PD1

static unsigned long bitTime_us = 0;
static unsigned long nextBitTime_us = 0;
static bool uartInitialized = false;

static inline void uartDelayBit(void)
{
    nextBitTime_us += bitTime_us;
    while ((nextBitTime_us - micros()) <= bitTime_us);
}

void HAL_UART_Init(uint32_t baudrate)
{
    // Configure TX pin as output
    TX_DDR |= (1 << TX_BIT);

    // Idle state = HIGH
    TX_PORT |= (1 << TX_BIT);

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
    TX_PORT &= ~(1 << TX_BIT);

    uartDelayBit();

    /* Data bits (LSB first) */
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x01)
            TX_PORT |= (1 << TX_BIT);
        else
            TX_PORT &= ~(1 << TX_BIT);

        uartDelayBit();
        byte >>= 1;
    }

    /* Stop bit (HIGH) */
    TX_PORT |= (1 << TX_BIT);
    uartDelayBit();

    //sei(); // re-enable interrupts
}

void HAL_UART_SendPayload(uint8_t *payload, size_t payloadSize)
{
    for (size_t i = 0; i < payloadSize; i++)
    {
        HAL_UART_SendByte(payload[i]);
    }
}

/* RX not implemented */
void HAL_UART_ReceivePayload(size_t messageMaxSize, char *message, size_t *messageSize)
{
    *messageSize = 0;
}

bool HAL_UART_GetByte(uint8_t *byte)
{
    return false;
}
