/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular, suporta a escrite e leitura de bytes sem manter estado interno
 * 
 * Inicialização retorna false se um dos parâmetros for nulo                            OK
 *      Buffer nulo                         OK
 *      Memória nula                        OK
 *      Tamanho da memória em bytes nula    OK
 * Inicialiação retorna true se houver sucesso                                          OK             
 * Buffer aponta estar vazio ao iniciar                                                 OK                   
 * Buffer retorna falso ao tentar ler com buffer vazio                                  OK
 * Buffer escreve valor e retorna true                                                  OK        
 * Buffer aponta não-vazio após inserir valor                                           OK
 * Buffer lê e retorna verdadeiro quando não vazio                                      OK
 * Buffer escreve e lê o mesmo byte aleatório                                           OK
 * Buffer aponta que está vazio após inserir e retirar byte aleatório                   OK
 * Buffer é reiniciado e após inserir byte e recriar buffer (vazio e leitura falsa)     OK
 * Buffer é reiniciado e após inserir byte e destruir buffer (vazio e leitura falsa)    OK
 * Buffer guarda dois bytes aleatórios e retorna os mesmos bytes                        OK
 * Buffer guarda dois bytes e mostra espaço sobrando corretamente após escrita          OK
 * Buffer guarda 4 bytes, lê 2 e mostra espaço sobrando corretamente                    OK
 * Buffer aponta que está cheio corretamente
 * Escrever N bytes retorna o número N
 * Tentar escrever N bytes com espaço M<N escreve M bytes e retorna M
 * Ler N bytes retorno o número N
 * Tentar ler N bytes com espaço M<N lê M bytes e retorna M               
 * Buffer aponta vazio após encher e esvaziar completamente 
 * Buffer realiza o wrap-around corretamente com bytes
 * Buffer mostra espaço sobrando com wrap-around

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

static CircularBuffer myBuffer;
static uint8_t  byteStorageBuffer[BUFFER_SIZE];

void resetStorageState(void)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        byteStorageBuffer[i] = 0;
    }
}

unsigned char randomByte(void) {
    /* rand() returns an `int` in [0, RAND_MAX] */
    int r = rand();
    /* map it into 0..255 */
    return (unsigned char)(r % 256);
}

TEST_SETUP(CircularBuffer)
{
    resetStorageState();

    srand((unsigned)time(NULL)); /* Gerar elementos aleatórios */

    CircularBuffer_Create(&myBuffer, byteStorageBuffer, BUFFER_SIZE);
}

TEST_TEAR_DOWN(CircularBuffer)
{

}

TEST(CircularBuffer, BadInitializationNullStruct)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(0, byteStorageBuffer, BUFFER_SIZE));
}

TEST(CircularBuffer, BadInitializationNullMemory)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(&myBuffer, 0, BUFFER_SIZE));
}

TEST(CircularBuffer, BadInitializationNullSize)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(&myBuffer, byteStorageBuffer, 0));
}

TEST(CircularBuffer, SucessfulInicializationReturnsTrue)
{
    TEST_ASSERT(CircularBuffer_Create(&myBuffer, byteStorageBuffer, BUFFER_SIZE));
}

TEST(CircularBuffer, IsEmptyAfterCreate)
{
    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
}

TEST(CircularBuffer, ReadWhileEmptyReturnsFalse)
{
    uint8_t readByte;
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

TEST(CircularBuffer, WriteWhenEmptyReturnsTrue)
{
    uint8_t writeByte = 'A';
    TEST_ASSERT(CircularBuffer_WriteOne(&myBuffer, writeByte));
}

TEST(CircularBuffer, ShowsNotEmptyAfterWrite)
{
    uint8_t writeByte = 'A';
    CircularBuffer_WriteOne(&myBuffer, writeByte);

    TEST_ASSERT_FALSE(CircularBuffer_IsEmpty(&myBuffer));
}

TEST(CircularBuffer, ReturnTrueOnReadAfterWrite)
{
    uint8_t writeByte = 'A';
    uint8_t readByte;
    CircularBuffer_WriteOne(&myBuffer, writeByte);

    TEST_ASSERT(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

TEST(CircularBuffer, WriteAndReadTheSameRandomByte)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_ReadOne(&myBuffer, &readByte);

    TEST_ASSERT_EQUAL_CHAR(writeByte, readByte);
}

TEST(CircularBuffer, BufferEmptyAgainAfterWriteAndRead)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_ReadOne(&myBuffer, &readByte);

    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
}

TEST(CircularBuffer, ReinitializeBufferOnSecondCreate)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, BUFFER_SIZE);

    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

TEST(CircularBuffer, ReinitializeBufferOnDestroy)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_Destroy(&myBuffer);
    
    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

TEST(CircularBuffer, Write2BytesAndRead2Bytes)
{
    uint8_t writeBytes[2] = {randomByte(), randomByte()};
    uint8_t readBytes[2];

    CircularBuffer_WriteMany(&myBuffer, writeBytes, 2);
    CircularBuffer_ReadMany(&myBuffer, readBytes, 2);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(writeBytes, readBytes, 2);
}

TEST(CircularBuffer, ShowSpaceRemainingAfterWrite)
{
    uint8_t writeBytes[4] = {randomByte(), randomByte(), randomByte(), randomByte()};

    CircularBuffer_WriteMany(&myBuffer, writeBytes, 4);

    TEST_ASSERT_EQUAL_INT64((BUFFER_SIZE-4), CircularBuffer_FreeSpace(&myBuffer));
}

TEST(CircularBuffer, ShowSpaceRemainingAfter4WriteAnd2Read)
{
    uint8_t writeBytes[4] = {randomByte(), randomByte(), randomByte(), randomByte()};
    uint8_t readBytes[2];

    CircularBuffer_WriteMany(&myBuffer, writeBytes, 4);
    CircularBuffer_ReadMany(&myBuffer, readBytes, 2);

    TEST_ASSERT_EQUAL_INT64((BUFFER_SIZE-2), CircularBuffer_FreeSpace(&myBuffer));
}