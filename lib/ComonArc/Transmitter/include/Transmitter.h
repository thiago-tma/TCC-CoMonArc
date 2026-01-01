#ifndef H_COMONARCTRANSMITTER_D
#define H_COMONARCTRANSMITTER_D

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TRANSMITTER_MAX_ATTACHED_CALLBACKS 3

typedef void (*TransmitCallback_t)(uint8_t * payload, size_t payloadSize);

void Transmitter_Create     (void);
void Transmitter_Destroy    (void);

bool Transmitter_AttachTransmitCallback(TransmitCallback_t channel);
/* bool Transmitter_DetachTransmitCallback(TransmitCallback_t channel); */

/*Transmit payload to attached*/
void Transmitter_TransmitPayload (uint8_t * payload, size_t payloadSize);

#endif /*H_COMONARCTRANSMITTER_D*/