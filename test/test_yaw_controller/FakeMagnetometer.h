#ifndef D_FAKEMAGNETOMETER_H
#define D_FAKEMAGNETOMETER_H

#include <stdint.h>
#include <Magnetometer.h>
#include <stdbool.h>

bool FakeMagnetometer_IsInitialized (void);
void FakeMagnetometer_SetReading (int16_t heading);

void FakeMagnetometer_Reset(void);

#endif /*D_FAKEMAGNETOMETER_H*/