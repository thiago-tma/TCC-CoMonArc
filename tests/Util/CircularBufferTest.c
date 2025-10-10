/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular flexível, com definição do tamanho do elemento que entra e sai,
 *      mas não suporta elementos de diferentes tamanhos.
 * 
 * Inicialização retorna false se um dos parâmetros for nulo    OK
 *      Espaço de memória nulo      OK
 *      Numero de elementos nulo    OK
 *      Tamanho de elemento nulo    OK
 * Inicialiação retorna true se houver sucesso                  OK
 * Inicialização altera ponteiro de CircularBuffer com sucesso  
 * Buffer aponta estar vazio ao iniciar                         OK
 * Buffer retorna falso ao tentar ler com buffer vazio          OK
 * Buffer guarda valor e retorna true                           OK
 * Buffer aponta não-vazio após inserir valor                   OK
 * Buffer guarda e retorna o mesmo byte aleatório
 * Buffer guarda e retorna um int32 aleatório                           
 * Buffer aponta que está vazio após inserir e retirar byte aleatório
 * Buffer aponta que está vazio após inserir e destruir buffer
 * Buffer guarda e retorna dois bytes diferentes
 * Buffer guarda e retorna dois int32 diferentes              
 * Buffer aponta que está cheio corretamente               
 * Buffer aponta vazio após encher e esvaziar completamente 
 * Buffer realiza o wrap-around corretamente com bytes
 * Buffer realiza o wrap-around correntamente com int32
 * Buffer inicializa, escreve e lê um elemento duas vezes em sequência,
 *      um de byte e outro de int32

*/

#include "unity.h"
#include "unity_fixture.h"
#include "CircularBuffer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 10

TEST_GROUP(CircularBuffer);

static uint8_t  byteStorageBuffer[BUFFER_SIZE];
static uint32_t intStorageBuffer[BUFFER_SIZE];

static CircularBuffer myBuffer = 0;

void resetStorageState(void)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        byteStorageBuffer[i] = 0;
        intStorageBuffer[i] = 0;
    }
}

unsigned char random_byte(void) {
    /* rand() returns an `int` in [0, RAND_MAX] */
    int r = rand();
    /* map it into 0..255 */
    return (unsigned char)(r % 256);
}

TEST_SETUP(CircularBuffer)
{
    resetStorageState();
    myBuffer= 0;                 /* Refresh CircularBuffer pointer*/

    srand((unsigned)time(NULL)); /* Gerar elementos aleatórios */
}

TEST_TEAR_DOWN(CircularBuffer)
{

}

TEST(CircularBuffer, BadInitializationNullMemory)
{
    TEST_ASSERT(!CircularBuffer_Create(&myBuffer, 0, 1, 1));

}

TEST(CircularBuffer, BadInitializationNullElementsNumber)
{
    TEST_ASSERT(!CircularBuffer_Create(&myBuffer, byteStorageBuffer, 0, 1));
}

TEST(CircularBuffer, BadInitializationNullElementSize)
{
    TEST_ASSERT(!CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 0));
}

TEST(CircularBuffer, CreateAndDestroy)
{
    TEST_ASSERT(CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1));
    TEST_ASSERT(CircularBuffer_Destroy(myBuffer));
}

TEST(CircularBuffer, BufferCreateSetsBufferPointer)
{
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1);

    TEST_ASSERT(myBuffer);
}

TEST(CircularBuffer, BufferEmptyWhenCreated)
{
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1);

    TEST_ASSERT(CircularBuffer_IsEmpty(myBuffer));
}

TEST(CircularBuffer, ReadingBufferWhenEmptyReturnsFalse)
{
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1);
    char readChar = 0;

    TEST_ASSERT(!CircularBuffer_Read(myBuffer, &readChar));
}

TEST(CircularBuffer, BufferStoresElementAndReturnsTrue)
{
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1);
    char writeChar = 'A';

    TEST_ASSERT(CircularBuffer_Write(myBuffer, &writeChar));
}

TEST(CircularBuffer, BufferNotEmptyAfterWrite)
{
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, 1, 1);
    char writeChar = 'A';
    CircularBuffer_Write(myBuffer, &writeChar);

    TEST_ASSERT_FALSE(CircularBuffer_IsEmpty(myBuffer));
}

TEST(CircularBuffer, BufferWriteAndReadTheSameByte)
{
    unsigned char randomWriteByte = random_byte();
    unsigned char randomReadByte;
    
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, BUFFER_SIZE, 1);
    CircularBuffer_Write(myBuffer, (void *)&randomWriteByte);
    CircularBuffer_Read(myBuffer, (void *)&randomReadByte);

    TEST_ASSERT_EQUAL_CHAR(randomWriteByte, randomReadByte);
}