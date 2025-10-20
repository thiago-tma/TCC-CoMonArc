#ifndef D_USERINTERFACE_H
#define D_USERINTERFACE_H

#include "TimeTypes.h"

typedef void (*triggerFunction)(void);

typedef enum component
{
    Led = 0,
    Buzzer,
    MAX_ACTUATOR_NUMBER
} actuator;

void UserInterface_Create           (void);
void UserInterface_Destroy          (void);

void UserInterface_Run              (void);

void UserInterface_BlinkComponent   (actuator  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff);

void UserInterface_AddButtonFunction (triggerFunction function);

#endif /*D_USERINTERFACE_H*/