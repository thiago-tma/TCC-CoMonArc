#include <Arduino.h>
#include <HAL/UART.h>

/* Implementação estável entre 9600 e 38400 de baudrate */

#define TX_PIN 1   // Arduino D1 (USB TX)
#define TX_PORT PORTD
#define TX_DDR  DDRD
#define TX_BIT  1

#define UART_BUFFER_SIZE 128

static volatile uint8_t txBuffer[UART_BUFFER_SIZE];
static volatile uint8_t txHead = 0;
static volatile uint8_t txTail = 0;

static volatile uint8_t currentByte = 0;
static volatile uint8_t bitIndex = 0;
static volatile bool transmitting = false;

// ===== Timer2 setup =====
static void timer2Start(uint32_t baudrate)
{
    cli();

    UCSR0B = 0; // disable USART

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    // CTC mode
    TCCR2A |= (1 << WGM21);

    // prescaler = 8
    TCCR2B |= (1 << CS21);

    /* 1/baudrate = prescaler*ocr/F_CPU */
    uint32_t ocr = (F_CPU / (8UL * baudrate)) - 1;
    OCR2A = (uint8_t)ocr;

    TIMSK2 |= (1 << OCIE2A);

    sei();
}

static void timer2Stop(void)
{
    TIMSK2 &= ~(1 << OCIE2A);
}

// ===== ISR =====
ISR(TIMER2_COMPA_vect)
{
    if (!transmitting)
    {
        if (txHead == txTail)
        {
            // idle high
            TX_PORT |= (1 << TX_BIT);
            return;
        }

        currentByte = txBuffer[txTail];
        txTail++;
        if (txTail >= UART_BUFFER_SIZE)
        {
            txTail = 0;
        }
        bitIndex = 0;
        transmitting = true;

        // start bit
        TX_PORT &= ~(1 << TX_BIT);
        return;
    }

    if (bitIndex < 8)
    {
        if (currentByte & 0x01)
            TX_PORT |= (1 << TX_BIT);
        else
            TX_PORT &= ~(1 << TX_BIT);

        currentByte >>= 1;
        bitIndex++;
    }
    else
    {
        // stop bit
        TX_PORT |= (1 << TX_BIT);
        transmitting = false;
    }
}

// ===== API =====

void HAL_UART_Init(uint32_t baudrate)
{
    // configure TX pin (D1)
    TX_DDR |= (1 << TX_BIT);
    TX_PORT |= (1 << TX_BIT); // idle high

    txHead = txTail = 0;
    transmitting = false;

    timer2Start(baudrate);
}

void HAL_UART_Deinit(void)
{
    timer2Stop();
}

void HAL_UART_SendPayload(uint8_t *payload, size_t payloadSize)
{
    for (size_t i = 0; i < payloadSize; i++)
    {
        while (((txHead + 1) % UART_BUFFER_SIZE) == txTail);

        cli();
        txBuffer[txHead] = payload[i];
        txHead++;
        if (txHead >= UART_BUFFER_SIZE)
        {
            txHead = 0;
        }
        sei();
    }

    // synchronous: wait until done
    while (txHead != txTail || transmitting);
}

// RX not implemented
void HAL_UART_ReceivePayload(size_t messageMaxSize, char *message, size_t *messageSize)
{
    *messageSize = 0;
}