#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H

#include <stdbool.h>
#include <stddef.h>

typedef struct CircularBufferStruct * CircularBuffer;

/* @param buffer            Ponteiro para o CircularBuffer a ser iniciado */
/* @param bufferMemory      Vetor de memória usado como buffer. O usuário deve garantir que o tamanho do buffer passado está correto*/
/* @param elementsNumber    Capacidade de elementos do buffer                                             */
/* @param elementSize       Número de bytes em um só elemento*/
/* Retorna true com sucesso, false se um dos parâmetros for nulo                                                          */
bool  CircularBuffer_Create   (CircularBuffer * buffer ,void * const bufferMemory, size_t elementsNumber, size_t elementSize);

/* Restante das funções abaixo supõem um parâmetro CircularBuffer buffer válido  */

/* Reinicia (Resets) o uso do buffer, tornando-o vazio  */
/* Não tenta desalocar/liberar a memória do buffer      */
/* Retorna 'true' com o sucesso da operação             */
bool             CircularBuffer_Destroy             (CircularBuffer buffer);

/* Retorna 'true' se vazio */
bool            CircularBuffer_IsEmpty             (CircularBuffer buffer);

/* Retorna 'true' se cheio */
bool            CircularBuffer_IsFull              (CircularBuffer buffer);

/* Retorna 'true' se valor foi inserido, 'false* se buffer está cheio, descartando o valor */
bool            CircularBuffer_Write               (CircularBuffer buffer, void const  * elementIn);

/* Retorna 'true' se valor foi lido, 'false* se buffer está vazio     */
bool            CircularBuffer_Read                (CircularBuffer buffer, void *  elementOut);


/* ####################################### FUNÇÕES DE EXTENSÃO #################################################### */

/* Função de execução rápida, somente quando elementos tem 1 byte de tamanho*/
/* Retorna 'true' se valor foi inserido, 'false* se buffer está cheio, descartando o valor */
/* NÃO IMPLEMENTADO */
bool            CircularBuffer_WriteByte               (CircularBuffer buffer, char elementIn);

/* Função de execução rápida, somente quando elementos tem 1 byte de tamanho*/
/* Retorna 'true' se valor foi lido, 'false* se buffer está vazio     */
/* NÃO IMPLEMENTADO */
bool            CircularBuffer_ReadByte                (CircularBuffer buffer, char *  elementOut);

#endif  /* D_CircularBuffer_H */