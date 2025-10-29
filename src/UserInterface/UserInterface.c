#include "UserInterface.h"
#include "GPIO.h"
#include "BSP_Pins.h"
#include "SoftTimer.h"

typedef struct 
{
    SoftTimer actuatorTimer;
    timeMicroseconds intervalOn;
    timeMicroseconds intervalOff;
    GPIO_Value_t currentState;
    unsigned int repetitions;
}Actuator_Scheduler_t;

static Actuator_Scheduler_t actuatorSchedulers[ACTUATOR_COUNT];
static GPIO_Pin_t * actuatorPins[ACTUATOR_COUNT];

static GPIO_Pin_t  *buttonPin;
static GPIO_Value_t buttonState = GPIO_VALUE_LOW, lastReading = GPIO_VALUE_LOW;
static SoftTimer    buttonTimer;
static triggerFunction buttonCallback = 0;

static void actuatorWrite(Actuator_t actuator, GPIO_Value_t value)
{
    if (actuatorPins[actuator])
    {
        GPIO_WritePin(*actuatorPins[actuator], value);
        actuatorSchedulers[actuator].currentState = value;
    }
}

void UserInterface_Create (void)
{
    GPIO_Create();

    actuatorPins[ACTUATOR_LED] = BSP_GetPin(BSP_PIN_LED);
    actuatorPins[ACTUATOR_BUZZER] = BSP_GetPin(BSP_PIN_BUZZER);

    buttonPin = BSP_GetPin(BSP_PIN_BUTTON);

    GPIO_ConfigPin(*actuatorPins[ACTUATOR_LED], GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*actuatorPins[ACTUATOR_BUZZER], GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    GPIO_ConfigPin(*buttonPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);
}

void UserInterface_Destroy (void)
{
    /* Turn off actuators and reset static memory */
    actuatorWrite(ACTUATOR_LED, GPIO_VALUE_LOW);
    actuatorWrite(ACTUATOR_BUZZER, GPIO_VALUE_LOW);
    
    for (int index = 0; index < ACTUATOR_COUNT; index++)
    {
        /* Define all elements of struct as zero */
        actuatorSchedulers[index] = (Actuator_Scheduler_t){0};
        actuatorPins[index] = 0;
    }
    
    buttonPin = 0;
    buttonState = GPIO_VALUE_LOW, lastReading = GPIO_VALUE_LOW;
    buttonTimer = (SoftTimer){0};
    buttonCallback = 0;
}

void checkActuatorsBlink(void)
{
    for (int index = 0; index < ACTUATOR_COUNT; index++)
    {
        /* Check if it's time to change state of actuator */
        if (SoftTimer_Check(&actuatorSchedulers[index].actuatorTimer))
        {
            if (actuatorSchedulers[index].currentState == GPIO_VALUE_HIGH)
            {
                /* Turn off actuator, reset timer to wait for next blink if needed */
                actuatorWrite(index, GPIO_VALUE_LOW);
                actuatorSchedulers[index].repetitions--;
                if (actuatorSchedulers[index].repetitions > 0)
                {
                    SoftTimer_Create(&actuatorSchedulers[index].actuatorTimer, actuatorSchedulers[index].intervalOff);
                }
            }
            else
            {
                /* Turn on actuator, reset timer to wait for turning off */
                actuatorWrite(index, GPIO_VALUE_HIGH);
                SoftTimer_Create(&actuatorSchedulers[index].actuatorTimer, actuatorSchedulers[index].intervalOn);
            }
        }
    }
}

void checkButton(void)
{
    GPIO_Value_t newReading;
    GPIO_ReadPin(*buttonPin, &newReading);

    if (newReading != lastReading && newReading != buttonState)
    {
        SoftTimer_Create(&buttonTimer, 50000);
    }

    if (SoftTimer_Check(&buttonTimer))
    {
        buttonState = newReading;
        if (buttonState && buttonCallback) buttonCallback();

        SoftTimer_Destroy(&buttonTimer);
    }

    lastReading = newReading;
}

void UserInterface_Run (void)
{
    checkActuatorsBlink();
    checkButton();
}

void UserInterface_BlinkComponent (Actuator_t  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff)
{
    if (repetitions == 0)
    {
        actuatorWrite(blinkActuator, GPIO_VALUE_LOW);
        actuatorSchedulers[blinkActuator].repetitions = repetitions;
        SoftTimer_Destroy(&actuatorSchedulers[blinkActuator].actuatorTimer);
        return;
    }

    /* Put pin on ACTIVE state and time when it should be turned off */
    actuatorWrite(blinkActuator, GPIO_VALUE_HIGH);
    SoftTimer_Create(&actuatorSchedulers[blinkActuator].actuatorTimer, intervalOn);

    actuatorSchedulers[blinkActuator].repetitions = repetitions;
    actuatorSchedulers[blinkActuator].intervalOn = intervalOn;
    actuatorSchedulers[blinkActuator].intervalOff = intervalOff;
}

void UserInterface_AddButtonFunction (triggerFunction function)
{
    buttonCallback = function;
}

void UserInterface_ReadButton (Button_State_t * state)
{
    *state = (buttonState == GPIO_VALUE_HIGH) ? BUTTON_ACTIVE : BUTTON_INACTIVE;
}