#ifndef D_YAWCONTROLLER_H
#define D_YAWCONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Módulo controla um servo motor realinhar a haste do servo à leitura de um magnetometro obtido pelo callback passado na inicialização */
/* Servo controle a haste em um angulo de 0º a 180º*/

#include <stdint.h>

/* Tempo entre realinhar a haste em 90º e definir a próxima leitura como referência */
#define YAWCONTROLLER_RESET_TIME_MILLISECONDS 500
#define YAWCONTROLLER_RESET_TIME_MICROSECONDS 500000

#ifdef UNIT_TEST
    #define YAWCONTROLLER_PROPORTIONAL_MULTIPLER 1
    #define YAWCONTROLLER_PROPORTIONAL_DIVIDER   1
#else
    #define YAWCONTROLLER_PROPORTIONAL_MULTIPLER -1
    #define YAWCONTROLLER_PROPORTIONAL_DIVIDER   2
#endif

typedef enum
{
    YAWCONTROLLER_OK,
    YAWCONTROLLER_ERROR_NOT_INITIALIZED,
    YAWCONTROLLER_ERROR_ALREADY_INITIALIZED,
}   YawController_Error_t;

/* Inicia sinal de controle e posiciona a haste em 90º do servo */
YawController_Error_t YawController_Create(void);

/* Desliga sinal de controle */
YawController_Error_t YawController_Destroy(void);

/* Aguarda o intervalo definido antes de utilizar a leitura do magnetômetro como referência para o controle */
/* Em seguida, controla o servo para conservar a direção de referência */
YawController_Error_t YawController_Run(void);

/* Realinha haste e define nova referência após um intervalo de tempo */
YawController_Error_t YawController_Reset(void);

#ifdef __cplusplus
}
#endif


#endif /*D_YAWCONTROLLER_H*/