#ifndef H_COMONARCTRANSMITTER_D
#define H_COMONARCTRANSMITTER_D

#ifdef __cplusplus
extern "C" {
#endif

#include <Logger/include/Logger.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP 3

typedef enum
{
    TRANSMITTER_CALLBACK_GROUP_INSTANT,
    TRANSMITTER_CALLBACK_GROUP_DELAYED
} Transmitter_Callback_Group_t;

typedef enum
{
    TRANSMITTER_OK,
    TRANSMITTER_ERROR_NOT_INITIALIZED,
    TRANSMITTER_ERROR_INITIALIZED,
    TRANSMITTER_ERROR_CALLBACK_CAPACITY_FULL
}   Transmitter_Error_t;

typedef void (*TransmitCallback_t)(Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

Transmitter_Error_t Transmitter_Create     (void);
Transmitter_Error_t Transmitter_Destroy    (void);

Transmitter_Error_t Transmitter_AttachTransmitCallback(TransmitCallback_t callback, Transmitter_Callback_Group_t group);
/* bool Transmitter_DetachTransmitCallback(TransmitCallback_t channel); */

/*Transmit payload to attached*/
Transmitter_Error_t Transmitter_Transmit (Transmitter_Callback_Group_t group , Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize);

#ifdef __cplusplus
}
#endif

#endif /*H_COMONARCTRANSMITTER_D*/