#ifndef D_MAGNETOMETER_H
#define D_MAGNETOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Magnetometer_Create();
void Magnetometer_Destroy();

/* Leitura assíncrona do sensor magnetômetro */
/* Traduz leitura para os formatos utilizados pelo sistema */
void Magnetometer_NewRead();

/* heading é uma variável de valor entre 0-359, indicando a direção no plano horizontal */
void Magnetometer_GetHeading(int16_t * heading);

#ifdef __cplusplus
}
#endif

#endif /*D_MAGNETOMETER_H*/