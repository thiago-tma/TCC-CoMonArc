/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular, suporta a escrite e leitura de bytes sem manter estado interno
 * 
 * Inicialização retorna false se um dos parâmetros for nulo
 *      Memória nula
 *      Tamanho da memória em bytes nula
 * Inicialiação retorna true se houver sucesso                
 * Buffer aponta estar vazio ao iniciar                       
 * Buffer retorna falso ao tentar ler com buffer vazio        
 * Buffer escreve valor e retorna true                         
 * Buffer aponta não-vazio após inserir valor       
 * Buffer lê e retorna verdadeiro quando não vazio          
 * Buffer escreve e lê o mesmo byte aleatório                                        
 * Buffer aponta que está vazio após inserir e retirar byte aleatório
 * Buffer aponta que está vazio após inserir byte e destruir buffer
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

static uint8_t  byteStorageBuffer[BUFFER_SIZE];
static uint32_t intStorageBuffer[BUFFER_SIZE];

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

    srand((unsigned)time(NULL)); /* Gerar elementos aleatórios */
}

TEST_TEAR_DOWN(CircularBuffer)
{

}

TEST(CircularBuffer, BadInitializationNullMemory)
{

}
