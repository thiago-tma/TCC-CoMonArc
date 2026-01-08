#ifndef D_FAKESERVOCONTROLLER_H
#define D_FAKESERVOCONTROLLER_H

#include <stdint.h>
#include <ServoController.h>
#include <stdbool.h>

uint16_t FakeServoController_ReadHeading (void);
bool FakeServoController_IsInitialized (void);

void FakeServoController_Reset(void);

#endif /*D_FAKESERVOCONTROLLER_H*/