/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular flexível, com definição do tamanho do elemento que entra e sai,
 *      mas não suporta elementos de diferentes tamanhos.
 * 
 * Inicialização retorna false se um dos parâmetros for nulo
 * Inicialiação retorna true se houver sucesso
 * Buffer aponta estar vazio ao iniciar                     
 * Buffer retorna falso ao tentar ler com buffer vazio      
 * Buffer guarda e retorna o mesmo byte
 * Buffer guarda e retorna um int32                   
 * Buffer aponta não-vazio após inserir valor               
 * Buffer aponta que está vazio após inserir e retirar valor
 * Buffer aponta que está vazio após inserir e destruir buffer
 * Buffer guarda e retorna um byte duas vezes
 * Buffer guarda e retorna um int32 duas vezes               
 * Buffer aponta que está cheio corretamente               
 * Buffer aponta vazio após encher e esvaziar completamente 
 * Buffer realiza o wrap-around corretamente com bytes
 * Buffer realizao o wrap-around correntamente com int32

*/

#include "unity.h"
#include "unity_fixture.h"
#include "CircularBuffer.h"
#include <stdbool.h>
#include <stdint.h>

TEST_GROUP(CircularBuffer);

static uint8_t  byteStorageBuffer[10];
static uint32_t intStorageBuffer[10];

static CircularBuffer myBuffer= 0;

void resetStorageState(void)
{
    for (int i = 0; i < 10; i++)
    {
        byteStorageBuffer[i] = 0;
        intStorageBuffer[i] = 0;
    }
}

TEST_SETUP(CircularBuffer)
{
    resetStorageState();
    myBuffer= 0;
}

TEST_TEAR_DOWN(CircularBuffer)
{

}

TEST(CircularBuffer, BadInitializationNullMemory)
{
    TEST_ASSERT(!CircularBuffer_Create(myBuffer, 0, 1, 1));

}

TEST(CircularBuffer, BadInitializationNullElementsNumber)
{
    TEST_ASSERT(!CircularBuffer_Create( myBuffer, byteStorageBuffer, 0, 1));
}

TEST(CircularBuffer, BadInitializationNullElementSize)
{
    TEST_ASSERT(!CircularBuffer_Create( myBuffer, byteStorageBuffer, 1, 0));
}

TEST(CircularBuffer, CreateAndDestroy)
{
    TEST_ASSERT(CircularBuffer_Create(myBuffer, byteStorageBuffer, 1, 1));
    TEST_ASSERT(CircularBuffer_Destroy(myBuffer));
}