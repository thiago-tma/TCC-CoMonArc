#include <stdbool.h>
#include <stdlib.h>

#include <YawController.h>
#include <ServoController.h>
#include <Magnetometer.h>
#include <SoftTimer.h>
#include <Logger/include/log_api.h>

static bool initialized = false;

static SoftTimer resetTimer;
static bool referenceSet = false;

static int16_t currentReference = 0;
static int16_t currentServoHeading  = 0;

static void setServoHeadingDeg (int16_t heading)
{
    if      (heading > 270) heading = 0;
    else if (heading > 180) heading = 180;
    else if (heading < -90 ) heading = 180;
    else if (heading < 0) heading = 0;
    
    currentServoHeading = heading;
    ServoController_SetServo(heading);
}

static void resetYawControl (void)
{
    SoftTimer_Create(&resetTimer, YAWCONTROLLER_RESET_TIME_MICROSECONDS);
    referenceSet = false;
}

YawController_Error_t YawController_Create(void)
{
    if (initialized) return YAWCONTROLLER_ERROR_ALREADY_INITIALIZED;
    
    ServoController_Create();
    /*setServoHeadingDeg(90);*/
    
    Magnetometer_Create();
    resetYawControl();

    initialized = true;

    return YAWCONTROLLER_OK;
}

YawController_Error_t YawController_Destroy(void)
{
    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;
    initialized = false;
    return YAWCONTROLLER_OK;
}

YawController_Error_t YawController_Run(void)
{
    int16_t readingError, controlError, magnetometerReading;

    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;


    if (!referenceSet)
    {
        setServoHeadingDeg(90);
        if (SoftTimer_Check(&resetTimer))
        {
            Magnetometer_GetHeading(&currentReference);
            referenceSet = true;
            log_servo_data_reference(currentReference);
            return YAWCONTROLLER_OK;
        }   
    }

    if (referenceSet)
    {
        /*Run P control loop*/
        Magnetometer_GetHeading(&magnetometerReading);

        /* Check if is shorter to turn the servo inside the [0,360º] interval or to cross the 0/360 point */
        readingError = magnetometerReading - currentReference;
        if (abs(readingError) <= (360-abs(readingError))) controlError = readingError;
        else 
        {
            controlError = 360 - abs(readingError);
            controlError *= (readingError > 0 ? -1 : 1);
        }

        setServoHeadingDeg(currentServoHeading+(controlError/2));
    }

    return YAWCONTROLLER_OK;
}

YawController_Error_t YawController_Reset(void)
{
    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;
    
    resetYawControl();

    return YAWCONTROLLER_OK;
}