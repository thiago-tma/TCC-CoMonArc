#include <Arduino.h>
#include <HAL/UART.h>
#include <SystemClock.h>
#include <SoftTimer.h>
#include <BSP_Pins.h>
#include <GPIO.h>

/* Write Baudrate limit, using delayMicros and no interrupts, is 2400 for the Arduino Uno */

#define HAL_UART_BITBANG_RECEIVE_BUFFER_SIZE 100
#define HAL_UART_BITBANG_DATA_BITS 8


static bool initialized = false;
static int callbackId = 0;
static uint32_t setBaudrate = 0;
static timeMicroseconds writeWaitPeriod = 0;
static timeMicroseconds readWaitPeriod = 0;
static timeMicroseconds readSkipPeriod = 0;


static GPIO_Pin_t * TXPin, * RXPin;

static volatile uint8_t receiveBuffer[HAL_UART_BITBANG_RECEIVE_BUFFER_SIZE];
static volatile uint16_t receiveBufferIndex = 0;

static SoftTimer writeTimer, readTimer, timeoutTimer;

typedef enum 
{
    RECEIVE_STATE_WAITING,
    RECEIVE_STATE_RECEIVING,
    RECEIVE_STATE_END
} Receive_State_t;

static volatile Receive_State_t RXState = RECEIVE_STATE_WAITING;

void receiveCallback (void)
{
    GPIO_Value_t readVal;
    static uint8_t receivedByte = 0;
    static uint8_t currentBit = 0;

    GPIO_ReadPin(*RXPin, &readVal);
    switch (RXState)
    {
    case RECEIVE_STATE_WAITING:
        if (RXState == RECEIVE_STATE_WAITING && readVal == LOW)
        {
            /* Beginning of the start bit */
            SoftTimer_Create(&readTimer, readSkipPeriod); /* Skip to sample in the middle of the next bit */
            currentBit = 0;
            RXState = RECEIVE_STATE_RECEIVING;
        }
        return;
    case RECEIVE_STATE_RECEIVING:
        if (SoftTimer_Check(&readTimer))
        {
            /* Middle of the [currentBit] data bit */
            receivedByte = (readVal << currentBit) | receivedByte;
            currentBit++;
            if (currentBit >= 8)    /* 8 data bits */
            {
                RXState = RECEIVE_STATE_END;
                SoftTimer_Create(&readTimer, readSkipPeriod);     /* Wait for the end of the stop bit    */
            }
            else SoftTimer_Create(&readTimer, readWaitPeriod);    /* Wait for the middle of the next bit */
        }
        return;
    case RECEIVE_STATE_END:
        if (SoftTimer_Check(&readTimer))
        {
            /*End of the stop bit*/
            receiveBuffer[receiveBufferIndex] = receivedByte;
            receivedByte = 0;
            RXState = RECEIVE_STATE_WAITING;
        }
        return;
    default:
        return;
    }
}

void HAL_UART_Init (uint32_t baudrate)
{
    if (initialized == true) return;

    setBaudrate = baudrate;
    writeWaitPeriod = 1000000UL/setBaudrate;
    //readWaitPeriod = 1000000UL/setBaudrate;
    //readSkipPeriod = (readWaitPeriod*3*1000000UL)/(2*setBaudrate);
    TXPin = BSP_GetPin(BSP_PIN_UART0_TX);
    RXPin =  BSP_GetPin(BSP_PIN_UART0_RX);
    GPIO_ConfigPin(*TXPin, GPIO_DIR_OUTPUT, GPIO_VALUE_HIGH);
    GPIO_ConfigPin(*RXPin, GPIO_DIR_INPUT, GPIO_VALUE_HIGH);

    //receiveBufferIndex = 0;
    //RXState = RECEIVE_STATE_WAITING;

    //SoftTimer_Create(&timeoutTimer, 500000UL);

    //callbackId = SystemClock_AddCallback(receiveCallback);
    initialized = true;
}

void HAL_UART_Deinit (void)
{
    if (initialized == false) return;

    //SystemClock_RemoveCallback(callbackId);

    GPIO_ConfigPin(*TXPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*RXPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);

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
        //cli();
        bitbangWrite(payload[index]);
        //sei();
    }   
}

void HAL_UART_ReceivePayload (size_t messageMaxSize, char * message, size_t * messageSize)
{
    *messageSize = 0;
    
    //Receive_State_t currentReceiveState;
    //uint16_t currentReceiveBufferIndex;
    //uint16_t index = 0;
    //cli();
    //currentReceiveState = RXState;
    //currentReceiveBufferIndex = receiveBufferIndex;    
    //if (currentReceiveBufferIndex != 0)
    //{
    //    while (index < currentReceiveBufferIndex && index < messageMaxSize)
    //    {
    //        message[index] = receiveBuffer[index];
    //        index++;
    //    }
    //    receiveBufferIndex = 0;
    //    SoftTimer_Reset(&timeoutTimer);
    //}
    //else
    //{
    //    if (currentReceiveState != RECEIVE_STATE_WAITING && SoftTimer_Check(&timeoutTimer))
    //    {
    //        /*RX protocol stuck, reset state */
    //        RXState = RECEIVE_STATE_WAITING;
    //    }
    //}
    //sei();
    
    //*messageSize = index;
    
}