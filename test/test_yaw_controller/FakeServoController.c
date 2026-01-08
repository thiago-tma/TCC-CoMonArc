#include "FakeServoController.h"
#include <stdbool.h>

static uint16_t currentHeading = 0;
static bool initialized = false;

ServoController_Error_t ServoController_Create(void)
{
    initialized = true;
}

ServoController_Error_t ServoController_Destroy(void)
{
    initialized = false;
}

ServoController_Error_t ServoController_SetServo(uint16_t heading)
{
    currentHeading = heading;
}

uint16_t FakeServoController_ReadHeading (void)
{
    return currentHeading;
}

bool FakeServoController_IsInitialized (void)
{
    return initialized;
}

void FakeServoController_Reset (void)
{
    initialized = false;
    currentHeading = 0;
}