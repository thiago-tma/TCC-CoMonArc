#include "UserInterface.h"
#include <GPIO.h>
#include <BSP_Pins.h>
#include <SoftTimer.h>
#include <Logger/include/log_api.h>

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

static bool initialized = false;

static void actuatorWrite(Actuator_t actuator, GPIO_Value_t value)
{
    if (actuatorPins[actuator])
    {
        GPIO_WritePin(*actuatorPins[actuator], value);
        actuatorSchedulers[actuator].currentState = value;
    }
}

UserInterface_Error_t UserInterface_Create (void)
{
    if (initialized) return USERINTERFACE_ERROR_ALREADY_INITIALIZED;

    GPIO_Create();

    actuatorPins[ACTUATOR_LED] = BSP_GetPin(BSP_PIN_LED);
    actuatorPins[ACTUATOR_BUZZER] = BSP_GetPin(BSP_PIN_BUZZER);

    buttonPin = BSP_GetPin(BSP_PIN_BUTTON);

    int errorVar = 0;
    errorVar += GPIO_ConfigPin(*actuatorPins[ACTUATOR_LED], GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    errorVar += GPIO_ConfigPin(*actuatorPins[ACTUATOR_BUZZER], GPIO_DIR_OUTPUT, GPIO_VALUE_LOW);
    errorVar += GPIO_ConfigPin(*buttonPin, GPIO_DIR_INPUT, GPIO_VALUE_LOW);

    if (errorVar)
    {
        log_user_interface_error_initialization_failed();
        return USERINTERFACE_ERROR_INITIALIZATION_FAILED;
    }

    initialized = true;
    log_user_interface_trace_initialized();

    return USERINTERFACE_OK;
}

UserInterface_Error_t UserInterface_Destroy (void)
{
    if (!initialized) return USERINTERFACE_ERROR_NOT_INITIALIZED;

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
    SoftTimer_Destroy(&buttonTimer);
    buttonCallback = 0;

    initialized = false;

    return USERINTERFACE_OK;
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
                else
                {
                    SoftTimer_Destroy(&actuatorSchedulers[index].actuatorTimer);
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
    /* Button debounce */

    GPIO_Value_t newReading;
    GPIO_ReadPin(*buttonPin, &newReading);

    if (newReading != lastReading)
    {
        if (newReading != buttonState) SoftTimer_Create(&buttonTimer, 50000);
        else SoftTimer_Destroy(&buttonTimer);
    }

    if (SoftTimer_Check(&buttonTimer))
    {
        buttonState = newReading;
        if (buttonState && buttonCallback) buttonCallback();

        SoftTimer_Destroy(&buttonTimer);
    }

    lastReading = newReading;
}

UserInterface_Error_t UserInterface_Run (void)
{
    if (!initialized) return USERINTERFACE_ERROR_NOT_INITIALIZED;

    checkActuatorsBlink();
    checkButton();

    log_user_interface_data_button_state(buttonState);

    return USERINTERFACE_OK;
}

UserInterface_Error_t UserInterface_BlinkComponent (Actuator_t  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff)
{
    if (!initialized) return USERINTERFACE_ERROR_NOT_INITIALIZED;

    if (repetitions == 0)
    {
        actuatorWrite(blinkActuator, GPIO_VALUE_LOW);
        actuatorSchedulers[blinkActuator].repetitions = repetitions;
        SoftTimer_Destroy(&actuatorSchedulers[blinkActuator].actuatorTimer);
        return USERINTERFACE_OK;
    }

    /* Put pin on ACTIVE state and time when it should be turned off */
    actuatorWrite(blinkActuator, GPIO_VALUE_HIGH);
    SoftTimer_Create(&actuatorSchedulers[blinkActuator].actuatorTimer, intervalOn);

    actuatorSchedulers[blinkActuator].repetitions = repetitions;
    actuatorSchedulers[blinkActuator].intervalOn = intervalOn;
    actuatorSchedulers[blinkActuator].intervalOff = intervalOff;

    return USERINTERFACE_OK;
}

UserInterface_Error_t UserInterface_AddButtonFunction (triggerFunction function)
{
    if (!initialized) return USERINTERFACE_ERROR_NOT_INITIALIZED;

    buttonCallback = function;

    return USERINTERFACE_OK;
}

UserInterface_Error_t UserInterface_ReadButton (Button_State_t * state)
{
    if (!initialized) return USERINTERFACE_ERROR_NOT_INITIALIZED;

    *state = (buttonState == GPIO_VALUE_HIGH) ? BUTTON_ACTIVE : BUTTON_INACTIVE;

    return USERINTERFACE_OK;
}