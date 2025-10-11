#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct CircularBufferStruct
{
    uint8_t * buffer;
    size_t bufferSize;
    size_t head;
    size_t tail;
} CircularBuffer;

/* @param buffer            Ponteiro para o buffer a ser iniciado */
/* @param bufferMemory      Vetor de memória usado como buffer. O usuário deve garantir que o tamanho do buffer passado está correto*/
/* @param bufferSize        Capacidade da memória passada em bytes*/
/* Retorna true com sucesso, false se um dos parâmetros for nulo                                                          */
bool  CircularBuffer_Create   (CircularBuffer * buffer , uint8_t * bufferMemory, size_t bufferSize);

/* Restante das funções abaixo supõem um parâmetro CircularBuffer válido  */

/* Reinicia (Resets) o uso do buffer, tornando-o vazio  */
/* Não tenta desalocar/liberar a memória do buffer      */
void            CircularBuffer_Destroy              (CircularBuffer * buffer);

/* Retorna 'true' se vazio */
bool            CircularBuffer_IsEmpty              (CircularBuffer * buffer);

/* Retorna 'true' se cheio */
bool            CircularBuffer_IsFull               (CircularBuffer * buffer);

/* Retorna espaço livre em bytes */
size_t          CircularBuffer_FreeSpace            (CircularBuffer * buffer);

/* Retorna 'true' se valor foi inserido, 'false* se buffer está cheio, descartando o valor */
bool            CircularBuffer_WriteOne             (CircularBuffer * buffer, uint8_t elementIn);

/* Retorna 'true' se valor foi lido, 'false* se buffer está vazio     */
bool            CircularBuffer_ReadOne              (CircularBuffer * buffer, uint8_t * elementOut);

/* Retorna o número de bytes escritos. Se buffer se tornar cheio, descarta o restante dos bytes */
size_t          CircularBuffer_WriteMany            (CircularBuffer * buffer, uint8_t * elementsIn, size_t lenght);

/* Retorna o número de bytes lidos */
size_t          CircularBuffer_ReadMany             (CircularBuffer * buffer, uint8_t * elementsOut, size_t lenght);

#endif  /* D_CircularBuffer_H */