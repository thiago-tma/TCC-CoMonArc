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
 * Buffer aponta que está cheio corretamente                                            OK
 * Retorna falso se tentar escrever com buffer cheio                                    OK
 * Escrever com buffer cheio descarta novo byte                                         OK
 * Função de escrever muitos retorna corretamente o número de bytes de fato escritos    OK
 * Função de ler muitos retorna corretamente o número de bytes de fato lidos            OK   
 * Buffer realiza o wrap-around corretamente com bytes                                  OK

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

size_t writeRandomBytes (uint8_t * bytes, size_t len)
{
    /* Gera bytes para o vetor passado, usa bytes para escrever e confere se foram escritos no buffer */
    for (size_t i = 0; i < len; i++)
    {
        bytes[i] = randomByte();
    }

    return (CircularBuffer_WriteMany(&myBuffer, bytes, len));
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
    uint8_t writeBytes[4];
    uint8_t readBytes[2];

    writeRandomBytes(writeBytes, 4);
    CircularBuffer_ReadMany(&myBuffer, readBytes, 2);

    TEST_ASSERT_EQUAL_INT64((BUFFER_SIZE-2), CircularBuffer_FreeSpace(&myBuffer));
}

TEST(CircularBuffer, BufferShowsFullCorrectly)
{
    uint8_t writeBytes[BUFFER_SIZE];
    uint8_t readByte;

    TEST_ASSERT_FALSE_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Shows full on initialization");

    writeRandomBytes(writeBytes, 1);
    TEST_ASSERT_FALSE_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Shows full after only writing 1 byte");
    
    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE-1, writeRandomBytes(writeBytes, BUFFER_SIZE-1), "A write operation failed");
    TEST_ASSERT_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Does not return true when full");

    /* Ler e escrever outro byte para colocar buffer na condição de wrap-around */
    CircularBuffer_ReadOne(&myBuffer, &readByte);       
    CircularBuffer_WriteOne(&myBuffer, randomByte());
    TEST_ASSERT_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Does not return true when full on wrap-around");
}

TEST(CircularBuffer, WriteWhileFullReturnsFalse)
{
    uint8_t writeBytes[BUFFER_SIZE];
    writeRandomBytes(writeBytes, BUFFER_SIZE);

    TEST_ASSERT_FALSE(CircularBuffer_WriteOne(&myBuffer, randomByte()));
}

TEST(CircularBuffer, WriteWhileFullDiscardsNewByte)
{
    uint8_t writeBytes[BUFFER_SIZE];
    uint8_t readBytes[BUFFER_SIZE];
    
    writeRandomBytes(writeBytes, BUFFER_SIZE);
    TEST_ASSERT_FALSE(CircularBuffer_WriteOne(&myBuffer, randomByte()));
    CircularBuffer_ReadMany(&myBuffer, readBytes, BUFFER_SIZE);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(writeBytes, readBytes, BUFFER_SIZE);
}

TEST(CircularBuffer, WriteManyReturnsNumberOfWrittenBytes)
{
    uint8_t writeBytes[BUFFER_SIZE];

    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE-5, writeRandomBytes(writeBytes, BUFFER_SIZE-5), "Check if buffer capacity is bigger than 5 bytes");
    TEST_ASSERT_EQUAL_INT64_MESSAGE(5, writeRandomBytes(writeBytes, BUFFER_SIZE), "Buffer must discard bytes when full");
}

TEST(CircularBuffer, ReadManyReturnsNumberOfReadBytes)
{
    uint8_t writeBytes[BUFFER_SIZE];
    uint8_t readBytes[BUFFER_SIZE];
    writeRandomBytes(writeBytes, BUFFER_SIZE);

    TEST_ASSERT_EQUAL_INT64(BUFFER_SIZE-3, CircularBuffer_ReadMany(&myBuffer, readBytes, BUFFER_SIZE-3));
    TEST_ASSERT_EQUAL_INT64(3, CircularBuffer_ReadMany(&myBuffer, readBytes, BUFFER_SIZE));
}

TEST(CircularBuffer, CheckWrapAroundFunction)
{
    uint8_t writeBytes[BUFFER_SIZE];
    uint8_t readBytes[BUFFER_SIZE];

    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE ,writeRandomBytes(writeBytes, BUFFER_SIZE), "Write without wrap error"); 
    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE ,CircularBuffer_ReadMany(&myBuffer, readBytes, BUFFER_SIZE), "Read without wrap error");
    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE ,writeRandomBytes(writeBytes, BUFFER_SIZE), "Write with wrap error"); 
    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE ,CircularBuffer_ReadMany(&myBuffer, readBytes, BUFFER_SIZE), "Read with wrap error");

    TEST_ASSERT_EQUAL_CHAR_ARRAY_MESSAGE(writeBytes, byteStorageBuffer, BUFFER_SIZE, "Write does not wrap around");
    TEST_ASSERT_EQUAL_CHAR_ARRAY_MESSAGE(readBytes, byteStorageBuffer, BUFFER_SIZE, "Read does not wrap around");

    TEST_ASSERT_EQUAL_INT64_MESSAGE(BUFFER_SIZE, CircularBuffer_FreeSpace(&myBuffer), "Available space left is incorrect");
}