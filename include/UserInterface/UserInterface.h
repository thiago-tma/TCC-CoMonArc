#ifndef D_USERINTERFACE_H
#define D_USERINTERFACE_H

#include "TimeTypes.h"

typedef void (*triggerFunction)(void);

typedef enum component
{
    ACTUATOR_LED = 0,
    ACTUATOR_BUZZER,
    ACTUATOR_COUNT
} actuator_t;

void UserInterface_Create           (void);
void UserInterface_Destroy          (void);

void UserInterface_Run              (void);

void UserInterface_BlinkComponent   (actuator_t  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff);

void UserInterface_AddButtonFunction (triggerFunction function);

#endif /*D_USERINTERFACE_H*/