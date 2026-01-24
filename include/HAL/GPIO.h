#ifndef D_GPIO_H
#define D_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

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
} GPIO_Int_Trigger_t;

typedef struct
{
    uint8_t port;
    uint8_t pin;
} GPIO_Pin_t;


typedef void (GPIO_Callback_t)(void);

/* Initialize the GPIO subsystem (if needed by hardware) */
GPIO_Status_t GPIO_Create(void);

GPIO_Status_t GPIO_Destroy(void);

/* Configure a given pin (by hardware spec) */
/* Value sets High or Low state for outputs and Pull-up or Pull-Down for Input*/
GPIO_Status_t GPIO_ConfigPin(GPIO_Pin_t channel, GPIO_Direction_t direction, GPIO_Value_t value);

/* Write value to a pin */
GPIO_Status_t GPIO_WritePin(GPIO_Pin_t channel, GPIO_Value_t value);

/* Read value from a pin */
GPIO_Status_t GPIO_ReadPin(GPIO_Pin_t channel, GPIO_Value_t *value);

/* Toggle output pin */
GPIO_Status_t GPIO_TogglePin(GPIO_Pin_t channel);

/* Attach  and enable interrupt on channel */
GPIO_Status_t GPIO_EnableInterrupt(GPIO_Pin_t channel, GPIO_Callback_t callback, GPIO_Int_Trigger_t triggerType );

/* Disable interrupt on channel */
GPIO_Status_t GPIO_DisableInterrupt(GPIO_Pin_t channel);

#ifdef __cplusplus
}
#endif

#endif /*D_GPIO_H*/
