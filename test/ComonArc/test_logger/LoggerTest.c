/* 
 * Lista de testes para ComonArc Logger
 * Objetivo: Filtrar, armazenar e enviar mensagens por streams de bytes
 * Descrição: Módulo recebe mensagem e armazena se filtro estiver configurado para permitir
 *            Realiza transferência síncrona quando desejado
 *            Capacidade de executar um callback ao receber uma mensagem de erro
 *            
 * Log de erro e Flush enviam mensagem                                                  OK
 * Log de outro nível e Flush não enviam mensagem                                       OK
 * Log sem inicializar módulo não envia mensagem                                        OK
 * Destruir módulo apaga logs armazenados                                               OK
 * Habilitar nível permite envio de mensagem do nível e subsistemas selecionados        OK
 * Habilitar nível (inclusivo) permite envio de níveis inferiores                       OK
 * Habilitar nível selecionado para todos os sistemas                                   OK
 * Habilitar nível inclusivo para todos os sistemas                                     OK
 * Armazenar e enviar mensagem, duas vezes separadamente                                OK
 * Log com payload transmite payload corretamente                                       OK
 * Tentar Log com buffer cheio causa mensagem de erro                                   OK
 *      Tentar múltiplas vezes causa somente uma única mensagem de erro                 OK
 * Mensagem de erro causa execução de callback caso seja registrado                     OK
 *      Não executa callback se mensagem de erro não estiver habilitada                 OK
 *      Erro de overflow também causa execução de callback                              OK
 * Destruir módulo apaga callback                                                       OK
*/

#include <unity.h>
#include <Logger/include/Logger.h>
#include "FakeTransmitter.h"

static size_t payloadTransmittedSize;
static uint8_t * payloadTransmitted; 
static int callbackVar = 0;

void testCallback (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    callbackVar++;
}

void setUp(void)
{
    Logger_Create();
    FakeTransmitter_ResetTransmitBuffer();
    payloadTransmitted = 0;
    payloadTransmittedSize = 0;
    callbackVar = 0;

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, true, false);
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
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0};
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, testID, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_LogAndFlushLowLevelMessageDoesNotTransmit (void)
{
    uint8_t testID = 10;
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, testID, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(0, payloadTransmittedSize);
}

void test_NoInitializationPreventsLogging (void)
{
    uint8_t testID = 10;
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0};
    Logger_Destroy();
    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, true, false);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, testID, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(0, payloadTransmittedSize);
}

void test_LoggerDestroyClearsLogs (void)
{
    uint8_t testID = 10;
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0};
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, testID, 0, 0);
    Logger_Destroy();
    Logger_Create();
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(0, payloadTransmittedSize);
}

void test_SetFilterEnablingLowerLevelMessageFromSingleSubsystemAndLevel (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0};

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, true, false);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_SetFilterInclusiveEnableAllLowerLevels (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0, LOGGER_START_BYTE, 11, 0};

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, true, true);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_SetFilterEnableLevelForAllSubsystems (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0, LOGGER_START_BYTE, 12, 0};

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_EVENT, true, false);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_SetFilterEnableInclusiveForAllSystems (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0, LOGGER_START_BYTE, 11, 0, LOGGER_START_BYTE, 12, 0, LOGGER_START_BYTE, 13, 0};

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_DATA, true, true);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_SendAndFlushMessageSeparatedelyTwice (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, 0, LOGGER_START_BYTE, 11, 0};
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0);
    Logger_Flush();
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 11, 0, 0);
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_LogAndFlushMessageWithPayload (void)
{
    uint8_t payload[] = {13, 23, 43, 53};
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, 10, sizeof(payload), 13, 23, 43, 53};
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload));
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);
}

void test_AddLoggerErrorWhenBufferOverflows (void)
{
    int payloadSize = LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE-LOGGER_MESSAGE_MIN_LENGHT;
    uint8_t expectedTransmit[LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE] = {LOGGER_START_BYTE, 10, payloadSize};
    uint8_t expectedErrorMessage[LOGGER_MESSAGE_MIN_LENGHT] = {LOGGER_START_BYTE, LOG_LOGGER_ERROR_BUFFER_OVERFLOW, 0};
    
    int index = LOGGER_MESSAGE_MIN_LENGHT;
    while (index < (LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE))
    {
        expectedTransmit[index] = index;
        index++;
    }
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, &expectedTransmit[LOGGER_MESSAGE_MIN_LENGHT], payloadSize);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, &expectedTransmit[LOGGER_MESSAGE_MIN_LENGHT], payloadSize); /* Overflow, no bytes kept */
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, &expectedTransmit[LOGGER_MESSAGE_MIN_LENGHT], payloadSize); /* Multiple attempts before a flush incurs in only one error message */
    Logger_Flush();

    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit)+LOGGER_MESSAGE_MIN_LENGHT, payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, sizeof(expectedTransmit));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedErrorMessage, &payloadTransmitted[LOGGER_MAX_BUFFER_SIZE-LOGGER_BUFFER_OVERFLOW_ERROR_SPACE], LOGGER_MESSAGE_MIN_LENGHT);
}

void test_ErrorCallbackOnErrorMessageReceivedIfCallbackRegistered (void)
{
    Logger_AttachErrorCallback(testCallback);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 11, 0, 0);   /* Mensagem de outro nível não deve ativar callback */

    TEST_ASSERT_EQUAL_INT(1, callbackVar);
}

void test_NoErrorCallbackIfErrorMessageIsNotEnabled (void)
{
    Logger_AttachErrorCallback(testCallback);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_ERROR, 10, 0, 0);   /* Error Messages not enabled*/

    TEST_ASSERT_EQUAL_INT(0, callbackVar);
}

void test_ErrorCallbackOnLoggerBufferOverflowError (void)
{
    uint8_t expectedTransmit[] = {LOGGER_START_BYTE, LOG_LOGGER_ERROR_BUFFER_OVERFLOW, 0};

    uint8_t emptyPayload[LOGGER_MAX_BUFFER_SIZE+1];
    Logger_AttachErrorCallback(testCallback);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_ERROR, 10, emptyPayload, sizeof(emptyPayload)); 
    Logger_Flush();

    TEST_ASSERT_EQUAL_INT(1, callbackVar);  /* Assert if callbakk is called */
    FakeTransmitter_GetTransmitBuffer(&payloadTransmitted, &payloadTransmittedSize);
    TEST_ASSERT_EQUAL_INT(sizeof(expectedTransmit), payloadTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedTransmit, payloadTransmitted, payloadTransmittedSize);    /* Assert if overflow error occured*/
}

void test_LoggerDestroyErasesAttachedCallback (void)
{
    Logger_AttachErrorCallback(testCallback);
    Logger_Destroy();
    Logger_Create();
    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, true, false);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0);

    TEST_ASSERT_EQUAL_INT(0, callbackVar);
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_LogAndFlushErrorMessage);
    RUN_TEST(test_LogAndFlushLowLevelMessageDoesNotTransmit);
    RUN_TEST(test_NoInitializationPreventsLogging);
    RUN_TEST(test_LoggerDestroyClearsLogs);
    RUN_TEST(test_SetFilterEnablingLowerLevelMessageFromSingleSubsystemAndLevel);
    RUN_TEST(test_SetFilterInclusiveEnableAllLowerLevels);
    RUN_TEST(test_SetFilterEnableLevelForAllSubsystems);
    RUN_TEST(test_SetFilterEnableInclusiveForAllSystems);
    RUN_TEST(test_SendAndFlushMessageSeparatedelyTwice);
    RUN_TEST(test_LogAndFlushMessageWithPayload);
    RUN_TEST(test_AddLoggerErrorWhenBufferOverflows);
    RUN_TEST(test_ErrorCallbackOnErrorMessageReceivedIfCallbackRegistered);
    RUN_TEST(test_NoErrorCallbackIfErrorMessageIsNotEnabled);
    RUN_TEST(test_ErrorCallbackOnLoggerBufferOverflowError);
    RUN_TEST(test_LoggerDestroyErasesAttachedCallback);

    UNITY_END();
}