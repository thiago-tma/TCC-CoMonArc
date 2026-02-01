#ifndef D_MAGNETOMETER_H
#define D_MAGNETOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    MAGNETOMETER_OK,
    MAGNETOMETER_ERROR_NOT_INITIALIZED,
    MAGNETOMETER_ERROR_ALREADY_INITIALIZED,
    MAGNETOMETER_ERROR_INITIALIZATION_FAILED,
    MAGNETOMETER_ERROR_NEWREAD_FAILED
} Magnetometer_Error_t;

Magnetometer_Error_t Magnetometer_Create(void);
Magnetometer_Error_t Magnetometer_Destroy(void);

/* Leitura assíncrona do sensor magnetômetro */
/* Traduz leitura para os formatos utilizados pelo sistema */
Magnetometer_Error_t Magnetometer_NewRead(void);

/* heading é uma variável de valor entre 0-359, indicando a direção no plano horizontal */
Magnetometer_Error_t Magnetometer_GetHeading(int16_t * heading);

#ifdef __cplusplus
}
#endif

#endif /*D_MAGNETOMETER_H*/