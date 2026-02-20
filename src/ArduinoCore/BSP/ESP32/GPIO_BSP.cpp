#include <Arduino.h>
#include <BSP_Pins.h>
#include <GPIO.h>

#define INVALID_PIN 255

/* ================= PIN MAP (ESP32) ================= */

static GPIO_Pin_t bsp_pin_map[BSP_PIN_COUNT] = {
    [BSP_PIN_LED]        = { .port = 0, .pin = 2  },   // Built-in LED (often GPIO2)
    [BSP_PIN_BUZZER]     = { .port = 0, .pin = 25 },
    [BSP_PIN_BUTTON]     = { .port = 0, .pin = 34 },  // input-only pin
    [BSP_PIN_ADC0]       = { .port = 0, .pin = 36 },  // ADC1_CH0
    [BSP_PIN_PWM0]       = { .port = 0, .pin = 26 },
    [BSP_PIN_SDA]        = { .port = 0, .pin = 21 },
    [BSP_PIN_SCL]        = { .port = 0, .pin = 22 },
    [BSP_PIN_UART0_RX]   = { .port = 0, .pin = 3  },
    [BSP_PIN_UART0_TX]   = { .port = 0, .pin = 1  },
};

/* ================= GPIO FUNCTIONS ================= */

GPIO_Pin_t * BSP_GetPin(BSP_Pin_Id_t id)
{
    if (id >= BSP_PIN_COUNT) return 0;
    if (bsp_pin_map[id].pin == INVALID_PIN) return 0;
    return &bsp_pin_map[id];
}

GPIO_Status_t GPIO_Create(void)
{
    return GPIO_OK;
}

GPIO_Status_t GPIO_Destroy(void)
{
    return GPIO_OK;
}

GPIO_Status_t GPIO_ConfigPin(GPIO_Pin_t channel, GPIO_Direction_t direction, GPIO_Value_t value)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;

    if (direction == GPIO_DIR_INPUT) pinMode(channel.pin, INPUT);
    else pinMode(channel.pin, OUTPUT);

    digitalWrite(channel.pin, (value == GPIO_VALUE_HIGH) ? HIGH : LOW);

    return GPIO_OK;
}

GPIO_Status_t GPIO_WritePin(GPIO_Pin_t channel, GPIO_Value_t value)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;

    digitalWrite(channel.pin, (value == GPIO_VALUE_HIGH) ? HIGH : LOW);

    return GPIO_OK;
}

GPIO_Status_t GPIO_ReadPin(GPIO_Pin_t channel, GPIO_Value_t *value)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;

    int readValue = digitalRead(channel.pin);
    *value = (readValue == HIGH) ? GPIO_VALUE_HIGH : GPIO_VALUE_LOW;

    return GPIO_OK;
}

GPIO_Status_t GPIO_TogglePin(GPIO_Pin_t channel)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;

    GPIO_Value_t readVal;
    GPIO_ReadPin(channel, &readVal);

    GPIO_WritePin(channel, (readVal == GPIO_VALUE_HIGH) ? GPIO_VALUE_LOW : GPIO_VALUE_HIGH);

    return GPIO_OK;
}

/* ================= INTERRUPTS ================= */

GPIO_Status_t GPIO_EnableInterrupt(GPIO_Pin_t channel, GPIO_Callback_t callback, GPIO_Int_Trigger_t triggerType)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;
    return GPIO_ERROR_HW_FAILURE;
}

GPIO_Status_t GPIO_DisableInterrupt(GPIO_Pin_t channel)
{
    if (channel.pin == INVALID_PIN) return GPIO_ERROR_INVALID_PIN;

    return GPIO_ERROR_HW_FAILURE;
}