
#ifndef D_FAKEGPIO_H
#define D_FAKEGPIO_H

#include "GPIO.h"
#include "BSP_Pins.h"

/* Reinicia estado interno para realizar novo teste */
void FakeGPIO_Reset(void);

bool FakeGPIO_GetInitialized(void);

GPIO_Direction_t FakeGPIO_GetPinDirection(BSP_Pin_Id_t pin);
GPIO_Value_t     FakeGPIO_GetPinValue(BSP_Pin_Id_t pin);


#endif /*D_FAKEGPIO_H*/