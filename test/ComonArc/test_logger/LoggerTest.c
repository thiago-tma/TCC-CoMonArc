/* 
 * Lista de testes para ComonArc Logger
 * Objetivo: Filtrar, armazenar e enviar mensagens por streams de bytes
 * Descrição: Módulo recebe mensagem e armazena se filtro estiver configurado para permitir
 *            Realiza transferência síncrona quando desejado
 *            Capacidade de executar um callback ao receber uma mensagem de erro
 *            
 * Log de erro e Flush enviam mensagem                                                  OK
 * Log de outro nível e Flush não enviam mensagem                                       OK
 * Log sem inicializar módulo não envia mensagem
 * Destruir módulo apaga logs armazenados
 * Habilitar nível permite envio de mensagem
 * Armazenar duas mensagens e enviar as duas em sequência
 * Log com payload transmite payload corretamente
 * Tentar Log com buffer cheio causa mensagem de erro
 *      Tentar múltiplas vezes causa somente uma única mensagem de erro
 * Mensagem de erro causa execução de callback caso seja registrado
 * Destruir módulo apaga callback
 * 
*/

#include <unity.h>
#include <Logger/include/Logger.h>
#include "FakeTransmitter.h"

static size_t payloadTransmittedSize;
static uint8_t * payloadTransmitted; 

void setUp(void)
{
    Logger_Create();
    FakeTransmitter_ResetTransmitBuffer();
    payloadTransmitted = 0;
    payloadTransmittedSize = 0;
}

void tearDown(void)
{
    Logger_Destroy();
}

void test_CreateAndDestroy (void)
{
    Logger_Create();
    Logger_Destroy();
}

void test_LogAndFlushErrorMessage (void)
{
    uint8_t testID = 10;
    uint8_t expectedTransmit[3] = {LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10};
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, testID, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(3, payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, 3);
}

void test_LogAndFlushLowLevelMessageDoesNotTransmit (void)
{
    uint8_t testID = 10;
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, testID, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(0, payloadTransmittedSize);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_LogAndFlushErrorMessage);
    RUN_TEST(test_LogAndFlushLowLevelMessageDoesNotTransmit);

    UNITY_END();
}