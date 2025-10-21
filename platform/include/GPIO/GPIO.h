#ifndef D_GPIO_H
#define D_GPIO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum 
{
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1
} GPIO_Direction_t;

typedef enum 
{
    GPIO_VALUE_LOW  = 0,
    GPIO_VALUE_HIGH = 1
} GPIO_Value_t;

typedef enum 
{
    GPIO_OK = 0,
    GPIO_ERROR_INVALID_PIN,
    GPIO_ERROR_UNINITIALIZED,
    GPIO_ERROR_HW_FAILURE
} GPIO_Status_t;

typedef enum 
{
    GPIO_INT_RISING_EDGE,
    GPIO_INT_FALLING_EDGE,
    GPIO_INT_TRANSITION
} GPIO_Int_Trigger_t

typedef void (GPIO_Callback_t)(void);

/* Initialize the GPIO subsystem (if needed by hardware) */
GPIO_Status_t GPIO_Create(void);

GPIO_Status_t GPIO_Destroy(void);

/* Configure a given pin (by hardware spec) */
GPIO_Status_t GPIO_ConfigPin(uint32_t channel, GPIO_Direction_t direction, bool pullupEnable);

/* Write value to a pin */
GPIO_Status_t GPIO_WritePin(uint32_t channel, GPIO_Value_t value);

/* Read value from a pin */
GPIO_Status_t GPIO_ReadPin(uint32_t channel, GPIO_Value_t *value);

/* Toggle output pin */
GPIO_Status_t GPIO_TogglePin(uint32_t channel);

/* Attach  and enable interrupt on channel */
GPIO_Status_t GPIO_EnableInterrupt(uint32_t channel, GPIO_Callback_t callback, GPIO_Trigger_t triggerType );

/* Disable interrupt on channel */
GPIO_Status_t GPIO_DisableInterrupt(uint32_t channel);

#endif /*D_GPIO_H*/
