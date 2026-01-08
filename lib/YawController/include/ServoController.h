#ifndef D_SERVOCONTROLLER_H
#define D_SERVOCONTROLLER_H

#include <stdint.h>

typedef enum
{
    SERVOCONTROLLER_OK
}   ServoController_Error_t;

ServoController_Error_t ServoController_Create(void);
ServoController_Error_t ServoController_Destroy(void);

/* heading from 0-180º*/
ServoController_Error_t ServoController_SetServo(int16_t heading);


#endif /*D_SERVOCONTROLLER_H*/