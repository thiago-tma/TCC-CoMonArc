#include "FakeMagnetometer.h"

static bool initialized = false;
static int16_t currentHeading = 0;

Magnetometer_Error_t Magnetometer_Create(void)
{
    initialized = true;
    return MAGNETOMETER_OK;
}
Magnetometer_Error_t Magnetometer_Destroy(void)
{
    return MAGNETOMETER_OK;
}

Magnetometer_Error_t Magnetometer_NewRead()
{
    return MAGNETOMETER_OK;
}

Magnetometer_Error_t Magnetometer_GetHeading(int16_t * heading)
{
    *heading = currentHeading;
    return MAGNETOMETER_OK;
}

bool FakeMagnetometer_IsInitialized (void)
{
    return initialized;
}

void FakeMagnetometer_SetReading (int16_t heading)
{
    currentHeading = heading;
}

void FakeMagnetometer_Reset(void)
{
    initialized = false;
    currentHeading = 0;
}