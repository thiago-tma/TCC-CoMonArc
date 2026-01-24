#ifndef D_HAL_UART_H
#define D_HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void HAL_UART_Init (uint32_t baudrate);
void HAL_UART_Deinit (void);

void HAL_UART_SendPayload (uint8_t * payload, size_t payloadSize);
void HAL_UART_ReceivePayload (size_t messageMaxSize, char * message, size_t * messageSize);

#ifdef __cplusplus
}
#endif

#endif /*D_HAL_UART_H*/