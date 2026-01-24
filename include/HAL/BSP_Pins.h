#ifndef D_BSP_PINS_H
#define D_BSP_PINS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "GPIO.h"

typedef enum {
  BSP_PIN_LED,
  BSP_PIN_BUZZER,
  BSP_PIN_BUTTON,
  BSP_PIN_ADC0,
  BSP_PIN_PWM0,
  BSP_PIN_SDA,    /* I2C data  */
  BSP_PIN_SCL,    /* I2C clock */
  BSP_PIN_UART0_RX,
  BSP_PIN_UART0_TX,
  BSP_PIN_COUNT
} BSP_Pin_Id_t;

GPIO_Pin_t * BSP_GetPin(BSP_Pin_Id_t id);

#ifdef __cplusplus
}
#endif

#endif /* D_BSP_PINS_H */