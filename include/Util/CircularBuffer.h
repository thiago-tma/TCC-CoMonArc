#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct CircularBufferStruct
{
    uint8_t * buffer;
    size_t bufferSize;
} * CircularBuffer;

/* @param buffer            Ponteiro para o buffer a ser iniciado */
/* @param bufferMemory      Vetor de memória usado como buffer. O usuário deve garantir que o tamanho do buffer passado está correto*/
/* @param bufferSize        Capacidade da memória passada em bytes*/
/* Retorna true com sucesso, false se um dos parâmetros for nulo                                                          */
bool  CircularBuffer_Create   (CircularBuffer buffer , uint8_t * bufferMemory, size_t bufferSize);

/* Restante das funções abaixo supõem um parâmetro CircularBuffer válido  */

/* Reinicia (Resets) o uso do buffer, tornando-o vazio  */
/* Não tenta desalocar/liberar a memória do buffer      */
/* Retorna 'true' com o sucesso da operação             */
bool            CircularBuffer_Destroy             (CircularBuffer buffer);

/* Retorna 'true' se vazio */
bool            CircularBuffer_IsEmpty             (CircularBuffer buffer);

/* Retorna 'true' se cheio */
bool            CircularBuffer_IsFull              (CircularBuffer buffer);

/* Retorna 'true' se valor foi inserido, 'false* se buffer está cheio, descartando o valor */
bool            CircularBuffer_Write               (CircularBuffer buffer, void const  * elementIn);

/* Retorna 'true' se valor foi lido, 'false* se buffer está vazio     */
bool            CircularBuffer_Read                (CircularBuffer buffer, void *  elementOut);


#endif  /* D_CircularBuffer_H */