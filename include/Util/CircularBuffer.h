#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H

#include <stdbool.h>

typedef struct CircularBufferStruct * CircularBuffer;

/* @param buffer            Ponteiro para o CircularBuffer a ser iniciado */
/* @param bufferStorage     Vetor de memória usado como buffer. O usuário deve garantir que o tamanho do buffer passado está correto*/
/* @param elementsNumber    Capacidade de elementos do buffer                                             */
/* @param elementSize       Número de bytes em um só elemento*/
/* Retorna 0 com sucesso, -1 se um dos parâmetros for nulo                                                          */
int  CircularBuffer_Create   (CircularBuffer buffer ,void * const bufferStorage, size_t elementsNumber, size_t elementSize);

/* Restante das funções abaixo supõem um parâmetro CircularBuffer buffer válido  */

/* Reinicia (Resets) o uso do buffer, tornando-o vazio  */
/* Não tenta desalocar/liberar a memória do buffer      */
/* Retorna 'true' com o sucesso da operação             */
bool             CircularBuffer_Destroy             (CircularBuffer buffer);

/* Retorna 'true' se vazio */
bool            CircularBuffer_IsEmpty             (CircularBuffer buffer);

/* Retorna 'true' se cheio */
bool            CircularBuffer_IsFull              (CircularBuffer buffer);

/* Retorna 'true' se valor foi inserido */
/* Retorna 'false* se buffer está cheio, descartando o valor*/
bool            CircularBuffer_Write               (CircularBuffer buffer, void const  * elementIn);

/* Retorna 'true' se valor foi lido     */
/* Retorna 'false* se buffer está vazio */
bool            CircularBuffer_Read                (CircularBuffer buffer, void *  elementOut);

#endif  /* D_CircularBuffer_H */