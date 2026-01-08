#include <YawController.h>
#include <stdbool.h>
#include <ServoController.h>
#include <Magnetometer.h>
#include <SoftTimer.h>

static bool initialized = false;

static SoftTimer resetTimer;

static bool referenceSet = false;
static uint16_t currentReferece = 0;
static uint16_t currentHeading  = 0;

static void setServoHeading (uint16_t heading)
{
    currentHeading = heading;
    ServoController_SetServo(heading);
}

YawController_Error_t YawController_Create(void)
{
    if (initialized) return YAWCONTROLLER_ERROR_ALREADY_INITIALIZED;
    
    ServoController_Create();
    ServoController_SetServo(90);
    
    Magnetometer_Create();

    SoftTimer_Create(&resetTimer, YAWCONTROLLER_RESET_TIME_MICROSECONDS);

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
    int32_t control;

    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;

    if (SoftTimer_Check(&resetTimer) && !referenceSet)
    {
        Magnetometer_GetHeading(&currentReferece);
        return YAWCONTROLLER_OK;
    }

    control = currentReferece - Magnetometer_GetHeading(); 

    return YAWCONTROLLER_OK;
}

YawController_Error_t YawController_Reset(void)
{
    if (!initialized) return YAWCONTROLLER_ERROR_NOT_INITIALIZED;
    
    return YAWCONTROLLER_OK;
}