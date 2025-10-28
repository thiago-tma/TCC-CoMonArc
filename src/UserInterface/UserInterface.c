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
static GPIO_Pin_t  *buttonPin;

static GPIO_Pin_t * actuatorPins[ACTUATOR_COUNT];


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
    
}

static void actuatorWrite(actuator_t actuator, GPIO_Value_t value)
{
    GPIO_WritePin(*actuatorPins[actuator], value);
    actuatorSchedulers[actuator].currentState = value;
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

void UserInterface_Run (void)
{
    checkActuatorsBlink();

}

void UserInterface_BlinkComponent (actuator_t  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff)
{
    /* Put pin on ACTIVE state and time when it should be turned off */
    GPIO_WritePin(*actuatorPins[blinkActuator], GPIO_VALUE_HIGH);
    actuatorSchedulers[blinkActuator].currentState = GPIO_VALUE_HIGH;

    SoftTimer_Create(&actuatorSchedulers[blinkActuator].actuatorTimer, intervalOn);

    actuatorSchedulers[blinkActuator].repetitions = repetitions;
    actuatorSchedulers[blinkActuator].intervalOn = intervalOn;
    actuatorSchedulers[blinkActuator].intervalOff = intervalOff;
}
