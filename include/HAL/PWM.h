#ifndef D_PWM_H
#define D_PWM_H

#include <stdint.h>
#include "GPIO.h"

typedef struct {
    GPIO_Pin_t  pin;
    uint32_t    frequencyHertz;
    uint8_t     dutyCyclePercentage;
} PWM_Config_t;

typedef struct {
    PWM_Config_t config;
    bool initialized;
} PWM_Handle_t;

void PWM_Create(PWM_Handle_t * PWMHandle, PWM_Config_t * PWMConfig);
void PWM_Destroy(PWM_Handle_t * PWMHandle);
void PWM_Write( PWM_Handle_t * PWMHandle,  uint8_t dutyCyclePercentage);

#endif /*D_PWM_H*/