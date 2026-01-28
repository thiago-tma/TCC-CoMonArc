#ifndef D_SERVOCONTROLLER_H
#define D_SERVOCONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    SERVOCONTROLLER_OK,
    SERVOCONTROLLER_ERROR_NOT_INITIALIZED,
    SERVOCONTROLLER_ERROR_INITIALIZED
}   ServoController_Error_t;

ServoController_Error_t ServoController_Create(void);
ServoController_Error_t ServoController_Destroy(void);

/* heading from 0-180º*/
ServoController_Error_t ServoController_SetServo(int16_t heading);

#ifdef __cplusplus
}
#endif

#endif /*D_SERVOCONTROLLER_H*/