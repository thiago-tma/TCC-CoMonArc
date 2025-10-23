#include "UserInterface.h"
#include "GPIO.h"
#include "BSP_Pins.h"

static GPIO_Pin_t *ledPin, *buzzerPin, *buttonPin;

void UserInterface_Create (void)
{
    GPIO_Create();

    ledPin = BSP_GetPin(BSP_PIN_LED);
    buzzerPin = BSP_GetPin(BSP_PIN_BUZZER);
    buttonPin = BSP_GetPin(BSP_PIN_BUTTON);

    GPIO_ConfigPin(*ledPin, GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*buzzerPin, GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*buttonPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);
}

void UserInterface_Destroy (void)
{
    
}