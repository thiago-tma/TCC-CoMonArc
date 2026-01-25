#include <Arduino.h>
#include <HAL/UART.h>
#include <SystemClock.h>
#include <SoftTimer.h>
#include <BSP_Pins.h>
#include <GPIO.h>

/* Baudrate limit, using delayMicros and no interrupts, is 4800 */

#define HAL_UART_BITBANG_RECEIVE_BUFFER_SIZE 100
#define HAL_UART_BITBANG_DATA_BITS 8


static bool initialized = false;
static int callbackId = 0;
static uint32_t setBaudrate = 0;
static timeMicroseconds writeWaitPeriod = 0;

static GPIO_Pin_t * TXPin, * readPin;

static volatile uint8_t receiveBuffer[HAL_UART_BITBANG_RECEIVE_BUFFER_SIZE];
static volatile uint16_t receiveBufferIndex = 0;

static SoftTimer writeTimer;

void receiveCallback (void)
{

}

void HAL_UART_Init (uint32_t baudrate)
{
    if (initialized == true) return;

    setBaudrate = baudrate;
    writeWaitPeriod = 1000000UL/setBaudrate;
    TXPin = BSP_GetPin(BSP_PIN_UART0_TX);
    readPin =  BSP_GetPin(BSP_PIN_UART0_RX);
    GPIO_ConfigPin(*TXPin, GPIO_DIR_OUTPUT, GPIO_VALUE_HIGH);
    GPIO_ConfigPin(*readPin, GPIO_DIR_INPUT, GPIO_VALUE_HIGH);

    receiveBufferIndex = 0;
    //callbackId = SystemClock_AddCallback(receiveCallback);
    initialized = true;
}

void HAL_UART_Deinit (void)
{
    if (initialized == false) return;

    SystemClock_RemoveCallback(callbackId);

    GPIO_ConfigPin(*TXPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*readPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);

    initialized = false;
}

/* delayMicros Implementation*/
void bitbangWrite(uint8_t byte)
{
    GPIO_WritePin(*TXPin, GPIO_VALUE_LOW);  /*Start bit*/
    delayMicroseconds(writeWaitPeriod);
    for (int index = 0; index < HAL_UART_BITBANG_DATA_BITS; index++)
    {
        GPIO_WritePin(*TXPin, (((byte >> index) & (uint8_t)1) ? GPIO_VALUE_HIGH : GPIO_VALUE_LOW));
        delayMicroseconds(writeWaitPeriod); 
    }
    GPIO_WritePin(*TXPin, GPIO_VALUE_HIGH); /*Stop bit*/
    delayMicroseconds(writeWaitPeriod);
}

/* SoftTimer Implementation */
//void bitbangWrite(uint8_t byte)
//{
//    //unsigned long waitTime = 1000000UL/setBaudrate;
//    SoftTimer_Create(&writeTimer, writeWaitPeriod);
//    GPIO_WritePin(*TXPin, GPIO_VALUE_LOW);  /*Start bit*/
//    while(!SoftTimer_Check(&writeTimer));
//    for (int index = 0; index < HAL_UART_BITBANG_DATA_BITS; index++)
//    {
//        GPIO_WritePin(*TXPin, (((byte >> index) & (uint8_t)1) ? GPIO_VALUE_HIGH : GPIO_VALUE_LOW));
//        while(!SoftTimer_Check(&writeTimer)); 
//    }
//    GPIO_WritePin(*TXPin, GPIO_VALUE_HIGH); /*Stop bit*/
//    while(!SoftTimer_Check(&writeTimer));
//}


void HAL_UART_SendPayload (uint8_t * payload, size_t payloadSize)
{
    if (initialized == false) return;
    for (size_t index = 0; index < payloadSize; index++)
    {
        cli();
        bitbangWrite(payload[index]);
        sei();
    }   
}

void HAL_UART_ReceivePayload (size_t messageMaxSize, char * message, size_t * messageSize)
{
    *messageSize = 0;
}