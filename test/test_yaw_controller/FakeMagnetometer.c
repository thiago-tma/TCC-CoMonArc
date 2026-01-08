#include "FakeMagnetometer.h"

static bool initialized = false;
static uint16_t currentHeading = 0;

void Magnetometer_Create()
{
    initialized = true;
}
void Magnetometer_Destroy();

void Magnetometer_NewRead();

void Magnetometer_GetHeading(uint16_t * heading)
{
    *heading = currentHeading;
}

bool FakeMagnetometer_IsInitialized (void)
{
    return initialized;
}

void FakeMagnetometer_SetReading (uint16_t heading)
{
    currentHeading = heading;
}

void FakeMagnetometer_Reset(void)
{
    initialized = false;
    currentHeading = 0;
}