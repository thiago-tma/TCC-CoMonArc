/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * Descrição: Buffer circular flexível, com definição do tamanho do elemento que entra e sai,
 *      mas não suporta elementos de diferentes tamanhos.
 * 
 * Inicialização retorna -1 se um dos parâmetros for nulo
 * Inicialiação retorna 1 se houver sucesso
 * Buffer aponta estar vazio ao iniciar                     
 * Buffer retorna falso ao tentar ler com buffer vazio      
 * Buffer guarda e retorna o mesmo valor                   
 * Buffer aponta não-vazio após inserir valor               
 * Buffer aponta que está vazio após inserir e retirar valor
 * Buffer guarda e retorna um valor duas vezes              
 * Buffer apontar que está cheio corretamente               
 * Buffer aponta vazio após encher e esvaziar completamente 
 * Testar funcionalidades e estado (vazio/com elementos/cheio) após de inserir e retirar
 *      o equivalente à capacidade (wrap-around do buffer circular)   
*/

#include "unity.h"
#include "unity_fixture.h"
#include "CircularBuffer.h"
#include <stdbool.h>

TEST_GROUP(CircularBuffer);

TEST_SETUP(CircularBuffer)
{

}

TEST_TEAR_DOWN(CircularBuffer)
{
    
}

TEST(CircularBuffer, CreateAndDestroy)
{

}