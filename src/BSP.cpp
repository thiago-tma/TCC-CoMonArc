#include <BSP_Pins.h>
#include <Arduino.h>

static GPIO_Pin_t bsp_pin_map[BSP_PIN_COUNT] = {
  [BSP_PIN_LED]  = { .port = 0, .pin = BSP_PIN_LED },
  [BSP_PIN_BUZZER] = { .port = 0, .pin = BSP_PIN_BUZZER},
  [BSP_PIN_BUTTON] = { .port = 0, .pin = BSP_PIN_BUTTON },
  [BSP_PIN_ADC0]  = {.port = 0, .pin = BSP_PIN_ADC0},
  [BSP_PIN_PWM0]  = {.port = 0, .pin = BSP_PIN_PWM0},
};

GPIO_Pin_t * BSP_GetPin(BSP_Pin_Id_t id) {
  if (id >= BSP_PIN_COUNT) return 0;
  return &bsp_pin_map[id];
}