#ifndef D_BSP_PINS_H
#define D_BSP_PINS_H

#include "GPIO.h"

typedef enum {
  BSP_PIN_LED,
  BSP_PIN_BUZZER,
  BSP_PIN_BUTTON,
  BSP_PIN_ADC0,
  BSP_PIN_PWM0,
  BSP_PIN_COUNT
} BSP_Pin_Id_t;

GPIO_Pin_t * BSP_GetPin(BSP_Pin_Id_t id);

#endif /* D_BSP_PINS_H */