/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular, suporta a escrite e leitura de bytes sem manter estado interno
 * 
 * Inicialização retorna false se um dos parâmetros for nulo    OK
 *      Buffer nulo                         OK
 *      Memória nula                        OK
 *      Tamanho da memória em bytes nula    OK
 * Inicialiação retorna true se houver sucesso   OK             
 * Buffer aponta estar vazio ao iniciar                       
 * Buffer retorna falso ao tentar ler com buffer vazio        
 * Buffer escreve valor e retorna true                         
 * Buffer aponta não-vazio após inserir valor       
 * Buffer lê e retorna verdadeiro quando não vazio          
 * Buffer escreve e lê o mesmo byte aleatório                                        
 * Buffer aponta que está vazio após inserir e retirar byte aleatório
 * Buffer é reiniciado e aponta que está vazio após inserir byte e recriar buffer
 * Buffer é reiniciado e aponta que está vazio após inserir byte e destruir buffer
 * Buffer guarda dois bytes aleatórios e retorna os mesmos bytes            
 * Buffer aponta que está cheio corretamente               
 * Buffer aponta vazio após encher e esvaziar completamente 
 * Buffer realiza o wrap-around corretamente com bytes

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

unsigned char random_byte(void) {
    /* rand() returns an `int` in [0, RAND_MAX] */
    int r = rand();
    /* map it into 0..255 */
    return (unsigned char)(r % 256);
}

TEST_SETUP(CircularBuffer)
{
    resetStorageState();

    srand((unsigned)time(NULL)); /* Gerar elementos aleatórios */
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