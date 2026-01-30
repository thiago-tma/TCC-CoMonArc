#ifndef H_COMONARCTRANSMITTER_D
#define H_COMONARCTRANSMITTER_D

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TRANSMITTER_MAX_ATTACHED_CALLBACKS 3

typedef enum
{
    TRANSMITTER_OK,
    TRANSMITTER_ERROR_NOT_INITIALIZED,
    TRANSMITTER_ERROR_INITIALIZED,
    TRANSMITTER_ERROR_CALLBACK_CAPACITY_FULL
}   Transmitter_Error_t;

typedef void (*TransmitCallback_t)(uint8_t * payload, size_t payloadSize);

Transmitter_Error_t Transmitter_Create     (void);
Transmitter_Error_t Transmitter_Destroy    (void);

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t channel);
/* bool Transmitter_DetachTransmitCallback(TransmitCallback_t channel); */

/*Transmit payload to attached*/
Transmitter_Error_t Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize);

#ifdef __cplusplus
}
#endif

#endif /*H_COMONARCTRANSMITTER_D*/