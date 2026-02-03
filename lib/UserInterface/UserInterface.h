#ifndef D_USERINTERFACE_H
#define D_USERINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "TimeTypes.h"

typedef void (*triggerFunction)(void);

typedef enum
{
    ACTUATOR_LED = 0,
    ACTUATOR_BUZZER,
    ACTUATOR_COUNT
} Actuator_t;

typedef enum
{
    BUTTON_ACTIVE,
    BUTTON_INACTIVE
}   Button_State_t;

typedef enum
{
    USERINTERFACE_OK,
    USERINTERFACE_ERROR_NOT_INITIALIZED,
    USERINTERFACE_ERROR_ALREADY_INITIALIZED,
    USERINTERFACE_ERROR_INITIALIZATION_FAILED
}   UserInterface_Error_t;

UserInterface_Error_t UserInterface_Create           (void);
UserInterface_Error_t UserInterface_Destroy          (void);

UserInterface_Error_t UserInterface_Run              (void);

UserInterface_Error_t UserInterface_BlinkComponent   (Actuator_t  blinkActuator, unsigned int repetitions, timeMicroseconds intervalOn, timeMicroseconds intervalOff);

UserInterface_Error_t UserInterface_ReadButton (Button_State_t * state);

UserInterface_Error_t UserInterface_AddButtonFunction (triggerFunction function);

#ifdef __cplusplus
}
#endif

#endif /*D_USERINTERFACE_H*/