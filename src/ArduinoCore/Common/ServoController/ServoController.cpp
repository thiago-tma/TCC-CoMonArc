#include <Arduino.h>
#include <Servo.h>
#include <ServoController.h>
#include <BSP_Pins.h>
#include <Logger/include/log_api.h>

static Servo servo;
static bool servoInitialized = false;
static uint8_t servoPin;

// ============================
// Create (initialize servo)
// ============================
ServoController_Error_t ServoController_Create(void)
{
    if (servoInitialized) return SERVOCONTROLLER_ERROR_INITIALIZED;

    servoPin = BSP_GetPin(BSP_PIN_PWM0)->pin;
    servo.attach(servoPin);
    servo.write(90); /* default to center position */
    servoInitialized = true;
    return SERVOCONTROLLER_OK;
}

// ============================
// Destroy (detach servo)
// ============================
ServoController_Error_t ServoController_Destroy(void)
{
    if (!servoInitialized) return SERVOCONTROLLER_ERROR_NOT_INITIALIZED;

    servo.detach();
    servoInitialized = false;

    return SERVOCONTROLLER_OK;
}

// ============================
// Set servo heading (0–180°)
// ============================
ServoController_Error_t ServoController_SetServo(int16_t heading)
{
    if (!servoInitialized) return SERVOCONTROLLER_ERROR_NOT_INITIALIZED;

    // Clamp range
    if (heading < 0) heading = 0;
    if (heading > 180) heading = 180;

    servo.write(heading);
    log_servo_data_direction(heading);

    return SERVOCONTROLLER_OK;
}