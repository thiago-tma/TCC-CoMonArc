#include "FakeGPIO.h"


typedef struct
{
    GPIO_Direction_t pinDirection;
    GPIO_Value_t    pinValue;
} PinState_t;

static PinState_t storedPinStates[BSP_PIN_COUNT];
static bool initialized = false;

void FakeGPIO_Reset(void) 
{
    /* Reiniciado para saída em alto para testar inicialização dos pinos */
    for (int i = 0; i < BSP_PIN_COUNT; i++)
    {
        storedPinStates[i].pinDirection = GPIO_DIR_OUTPUT;
        storedPinStates[i].pinValue = GPIO_VALUE_HIGH;
    }

    initialized = false;
}

bool FakeGPIO_GetInitialized(void)
{
    return initialized;
}

GPIO_Direction_t FakeGPIO_GetPinDirection(BSP_Pin_Id_t pin)
{
    return (storedPinStates[pin].pinDirection);
}

GPIO_Value_t FakeGPIO_GetPinValue(BSP_Pin_Id_t pin)
{
    return (storedPinStates[pin].pinValue);
}

GPIO_Status_t GPIO_Create(void)
{
    initialized = true;
    
    return GPIO_OK;
}

GPIO_Status_t GPIO_ConfigPin(GPIO_Pin_t channel, GPIO_Direction_t direction, GPIO_Value_t value)
{
    if (!initialized) return GPIO_ERROR_HW_FAILURE;

    storedPinStates[channel.pin].pinDirection = direction;
    storedPinStates[channel.pin].pinValue = value;

    return GPIO_OK;
}

GPIO_Status_t GPIO_WritePin(GPIO_Pin_t channel, GPIO_Value_t value)
{
    storedPinStates[channel.pin].pinValue = value;

    return GPIO_OK;
}