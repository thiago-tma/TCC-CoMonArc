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

#include <unity.h>
#include <CircularBuffer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define CB_TEST_BUFFERSIZE 10

static CircularBuffer myBuffer;
static uint8_t  byteStorageBuffer[CB_TEST_BUFFERSIZE];

void resetStorageState(void)
{
    for (int i = 0; i < CB_TEST_BUFFERSIZE; i++)
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

void setUp(void)
{
    resetStorageState();

    srand((unsigned)time(NULL)); /* Gerar elementos aleatórios */

    CircularBuffer_Create(&myBuffer, byteStorageBuffer, CB_TEST_BUFFERSIZE);
}

void tearDown(void)
{

}

void test_BadInitializationNullStruct (void)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(0, byteStorageBuffer, CB_TEST_BUFFERSIZE));
}

void test_BadInitializationNullMemory (void)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(&myBuffer, 0, CB_TEST_BUFFERSIZE));
}

void test_BadInitializationNullSize (void)
{
    TEST_ASSERT_FALSE(CircularBuffer_Create(&myBuffer, byteStorageBuffer, 0));
}

void test_SucessfulInicializationReturnsTrue (void)
{
    TEST_ASSERT(CircularBuffer_Create(&myBuffer, byteStorageBuffer, CB_TEST_BUFFERSIZE));
}

void test_IsEmptyAfterCreate (void)
{
    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
}

void test_ReadWhileEmptyReturnsFalse (void)
{
    uint8_t readByte;
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

void test_WriteWhenEmptyReturnsTrue (void)
{
    uint8_t writeByte = 'A';
    TEST_ASSERT(CircularBuffer_WriteOne(&myBuffer, writeByte));
}

void test_ShowsNotEmptyAfterWrite (void)
{
    uint8_t writeByte = 'A';
    CircularBuffer_WriteOne(&myBuffer, writeByte);

    TEST_ASSERT_FALSE(CircularBuffer_IsEmpty(&myBuffer));
}

void test_ReturnTrueOnReadAfterWrite (void)
{
    uint8_t writeByte = 'A';
    uint8_t readByte;
    CircularBuffer_WriteOne(&myBuffer, writeByte);

    TEST_ASSERT(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

void test_WriteAndReadTheSameRandomByte (void)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_ReadOne(&myBuffer, &readByte);

    TEST_ASSERT_EQUAL_CHAR(writeByte, readByte);
}

void test_BufferEmptyAgainAfterWriteAndRead (void)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_ReadOne(&myBuffer, &readByte);

    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
}

void test_ReinitializeBufferOnSecondCreate (void)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_Create(&myBuffer, byteStorageBuffer, CB_TEST_BUFFERSIZE);

    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

void test_ReinitializeBufferOnDestroy (void)
{
    uint8_t writeByte = randomByte();
    uint8_t readByte;

    CircularBuffer_WriteOne(&myBuffer, writeByte);
    CircularBuffer_Destroy(&myBuffer);
    
    TEST_ASSERT(CircularBuffer_IsEmpty(&myBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_ReadOne(&myBuffer, &readByte));
}

void test_Write2BytesAndRead2Bytes (void)
{
    uint8_t writeBytes[2] = {randomByte(), randomByte()};
    uint8_t readBytes[2];

    CircularBuffer_WriteMany(&myBuffer, writeBytes, 2);
    CircularBuffer_ReadMany(&myBuffer, readBytes, 2);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(writeBytes, readBytes, 2);
}

void test_ShowSpaceRemainingAfterWrite (void)
{
    uint8_t writeBytes[4] = {randomByte(), randomByte(), randomByte(), randomByte()};

    CircularBuffer_WriteMany(&myBuffer, writeBytes, 4);

    TEST_ASSERT_EQUAL_INT64((CB_TEST_BUFFERSIZE-4), CircularBuffer_FreeSpace(&myBuffer));
}

void test_ShowSpaceRemainingAfter4WriteAnd2Read (void)
{
    uint8_t writeBytes[4];
    uint8_t readBytes[2];

    writeRandomBytes(writeBytes, 4);
    CircularBuffer_ReadMany(&myBuffer, readBytes, 2);

    TEST_ASSERT_EQUAL_INT64((CB_TEST_BUFFERSIZE-2), CircularBuffer_FreeSpace(&myBuffer));
}

void test_BufferShowsFullCorrectly (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];
    uint8_t readByte;

    TEST_ASSERT_FALSE_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Shows full on initialization");

    writeRandomBytes(writeBytes, 1);
    TEST_ASSERT_FALSE_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Shows full after only writing 1 byte");
    
    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE-1, writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE-1), "A write operation failed");
    TEST_ASSERT_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Does not return true when full");

    /* Ler e escrever outro byte para colocar buffer na condição de wrap-around */
    CircularBuffer_ReadOne(&myBuffer, &readByte);       
    CircularBuffer_WriteOne(&myBuffer, randomByte());
    TEST_ASSERT_MESSAGE(CircularBuffer_IsFull(&myBuffer), "Does not return true when full on wrap-around");
}

void test_WriteWhileFullReturnsFalse (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];
    writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE);

    TEST_ASSERT_FALSE(CircularBuffer_WriteOne(&myBuffer, randomByte()));
}

void test_WriteWhileFullDiscardsNewByte (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];
    uint8_t readBytes[CB_TEST_BUFFERSIZE];
    
    writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE);
    TEST_ASSERT_FALSE(CircularBuffer_WriteOne(&myBuffer, randomByte()));
    CircularBuffer_ReadMany(&myBuffer, readBytes, CB_TEST_BUFFERSIZE);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(writeBytes, readBytes, CB_TEST_BUFFERSIZE);
}

void test_WriteManyReturnsNumberOfWrittenBytes (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];

    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE-5, writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE-5), "Check if buffer capacity is bigger than 5 bytes");
    TEST_ASSERT_EQUAL_INT64_MESSAGE(5, writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE), "Buffer must discard bytes when full");
}

void test_ReadManyReturnsNumberOfReadBytes (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];
    uint8_t readBytes[CB_TEST_BUFFERSIZE];
    writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE);

    TEST_ASSERT_EQUAL_INT64(CB_TEST_BUFFERSIZE-3, CircularBuffer_ReadMany(&myBuffer, readBytes, CB_TEST_BUFFERSIZE-3));
    TEST_ASSERT_EQUAL_INT64(3, CircularBuffer_ReadMany(&myBuffer, readBytes, CB_TEST_BUFFERSIZE));
}

void test_CheckWrapAroundFunction (void)
{
    uint8_t writeBytes[CB_TEST_BUFFERSIZE];
    uint8_t readBytes[CB_TEST_BUFFERSIZE];

    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE ,writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE), "Write without wrap error"); 
    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE ,CircularBuffer_ReadMany(&myBuffer, readBytes, CB_TEST_BUFFERSIZE), "Read without wrap error");
    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE ,writeRandomBytes(writeBytes, CB_TEST_BUFFERSIZE), "Write with wrap error"); 
    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE ,CircularBuffer_ReadMany(&myBuffer, readBytes, CB_TEST_BUFFERSIZE), "Read with wrap error");

    TEST_ASSERT_EQUAL_CHAR_ARRAY_MESSAGE(writeBytes, byteStorageBuffer, CB_TEST_BUFFERSIZE, "Write does not wrap around");
    TEST_ASSERT_EQUAL_CHAR_ARRAY_MESSAGE(readBytes, byteStorageBuffer, CB_TEST_BUFFERSIZE, "Read does not wrap around");

    TEST_ASSERT_EQUAL_INT64_MESSAGE(CB_TEST_BUFFERSIZE, CircularBuffer_FreeSpace(&myBuffer), "Available space left is incorrect");
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_BadInitializationNullMemory);
    RUN_TEST(test_BadInitializationNullMemory);
    RUN_TEST(test_BadInitializationNullSize);
    RUN_TEST(test_SucessfulInicializationReturnsTrue);
    RUN_TEST(test_IsEmptyAfterCreate);
    RUN_TEST(test_ReadWhileEmptyReturnsFalse);
    RUN_TEST(test_WriteWhenEmptyReturnsTrue);
    RUN_TEST(test_ShowsNotEmptyAfterWrite);
    RUN_TEST(test_ReturnTrueOnReadAfterWrite);
    RUN_TEST(test_WriteAndReadTheSameRandomByte);
    RUN_TEST(test_BufferEmptyAgainAfterWriteAndRead);
    RUN_TEST(test_ReinitializeBufferOnSecondCreate);
    RUN_TEST(test_ReinitializeBufferOnDestroy);
    RUN_TEST(test_Write2BytesAndRead2Bytes);
    RUN_TEST(test_ShowSpaceRemainingAfterWrite);
    RUN_TEST(test_ShowSpaceRemainingAfter4WriteAnd2Read);
    RUN_TEST(test_BufferShowsFullCorrectly);
    RUN_TEST(test_WriteWhileFullReturnsFalse);
    RUN_TEST(test_WriteWhileFullDiscardsNewByte);
    RUN_TEST(test_WriteManyReturnsNumberOfWrittenBytes);
    RUN_TEST(test_ReadManyReturnsNumberOfReadBytes);
    RUN_TEST(test_CheckWrapAroundFunction);

    UNITY_END();
}