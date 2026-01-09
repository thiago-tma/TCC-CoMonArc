#include <YawController.h>
#include <stdbool.h>
#include <ServoController.h>
#include <Magnetometer.h>
#include <SoftTimer.h>

static bool initialized = false;

static SoftTimer resetTimer;
static bool referenceSet = false;

static int16_t currentReference = 0;
static int16_t currentServoHeading  = 0;

static void setServoHeading (int16_t heading)
{
    if (heading > 180) heading = 180;
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
    /*setServoHeading(90);*/
    
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
    int16_t controlError, magnetometerReading;

    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;


    if (!referenceSet)
    {
        setServoHeading(90);
        if (SoftTimer_Check(&resetTimer))
        {
            Magnetometer_GetHeading(&currentReference);
            referenceSet = true;
            return YAWCONTROLLER_OK;
        }   
    }

    if (referenceSet)
    {
        /*Run P control loop*/
        Magnetometer_GetHeading(&magnetometerReading);
        controlError = magnetometerReading - currentReference;
        setServoHeading(currentServoHeading-controlError);
    }

    return YAWCONTROLLER_OK;
}

YawController_Error_t YawController_Reset(void)
{
    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;
    
    resetYawControl();

    return YAWCONTROLLER_OK;
}