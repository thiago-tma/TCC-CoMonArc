/* 
 * Lista de testes para ComonArc Logger
 * Objetivo: Filtrar e encaminhar mensagens para o transmitter
 * Descrição: Módulo recebe mensagem, repassa ou não com base no filtro
 *            Mensagens são repassadas imediatamente(Log) e/ou repassadas posteriormente (flush)
 *            
 * Chamar funções sem inicializar retorna erro                                          OK
 * Primeira inicialização retorna OK, próximas inicializações retoram aviso             OK  
 * Deinicialização                                                                      OK
 * Módulo inicializa dependências durante inicialização                                 OK
 * 
 * Modo imediato                               
 *      Mensagem imediata é filtrada e retorna aviso                                    OK
 *      Mensagem após habilitar filtro é enviada                                        OK
 *      Mensagem imediata somente nos modos permitidos                                  OK
 *      Destruir módulo reinicia filtro                                                 OK
 *      Habilitar nível (inclusivo) permite envio de níveis inferiores                  OK      
 *      Habilitar nível selecionado para todos os sistemas                              OK    
 *      Habilitar nível inclusivo para todos os sistemas                                OK   
 *      Log com payload transmite payload corretamente                                  OK
 * 
 * Modo adiado
 *      Mensagem adiada filtrada e não-filtrada (armazenada)                            OK
 *      Mensagens adiadas somente nos modos permitidos                                  OK
 *      Mensagens não sao enviadas de imediato                                          OK
 *      Dois Logs com payload transmite payload corretamente                            OK             
 *      Log sem espaço no buffer descarta mensagem e retorna erro                       OK
 *      Flush após overflow de buffer retorna  erro                                     OK
 * 
 * Modo misto
 *      Enviar mensagem imediata e mensagem adiada                       
*/

#include <unity.h>
#include <Logger/include/Logger.h>
#include "FakeTransmitter.h"

static size_t messagesTransmittedSize;
static uint8_t * messagesTransmitted; 
static int callbackVar = 0;

static uint8_t expectedMessage[200];
static size_t expectedMessageIndex;

static void checkTransmittedMessages(Transmitter_Callback_Group_t group)
{
    FakeTransmitter_GetTransmitBuffer(group,&messagesTransmitted, &messagesTransmittedSize);
    TEST_ASSERT_EQUAL_INT(expectedMessageIndex, messagesTransmittedSize);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage, messagesTransmitted, messagesTransmittedSize);
}

static void appendExpectedMessage(Log_Subsystem_t origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    expectedMessage[expectedMessageIndex++] = origin;
    expectedMessage[expectedMessageIndex++] = level;
    expectedMessage[expectedMessageIndex++] = messageID;

    size_t payloadIndex = 0;
    while(payloadIndex < payloadSize)
    {
        expectedMessage[(expectedMessageIndex)++] = payload[payloadIndex];
        payloadIndex++;
    }
    expectedMessage[expectedMessageIndex++] = payloadSize;
}

static void setLoggerMode (Logger_Mode_t mode)
{
    Logger_Destroy();
    Logger_Create(mode);
    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, true, false);
}

void setUp(void)
{
    FakeTransmitter_Reset();
    FakeTransmitter_ResetTransmitBuffer();
    Logger_Create(LOGGER_MODE_INSTANT);
    messagesTransmitted = 0;
    messagesTransmittedSize = 0;
    expectedMessageIndex = 0;

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, true, false);
}

void tearDown(void)
{
    Logger_Destroy();
}

void test_CreateAndDestroy (void)
{
    Logger_Create(0);
    Logger_Destroy();
}

void test_CallingInterfaceWithoutInitializationReturnsError (void)
{
    Logger_Destroy();
    TEST_ASSERT_EQUAL_MESSAGE(LOGGER_ERROR_NOT_INITIALIZED, Logger_Destroy(), "function 1");
    TEST_ASSERT_EQUAL_MESSAGE(LOGGER_ERROR_NOT_INITIALIZED, Logger_Log(0,0,0,0,0), "function 2");
    TEST_ASSERT_EQUAL_MESSAGE(LOGGER_ERROR_NOT_INITIALIZED, Logger_SetFilter(0,0,0,0), "function 3");
    TEST_ASSERT_EQUAL_MESSAGE(LOGGER_ERROR_NOT_INITIALIZED, Logger_ResetFilter(), "function 4"); 
    TEST_ASSERT_EQUAL_MESSAGE(LOGGER_ERROR_NOT_INITIALIZED, Logger_Flush(), "function 5");
}

void test_FirstAndNextInitializations (void)
{
    Logger_Destroy();
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Create(0));
    TEST_ASSERT_EQUAL(LOGGER_ERROR_ALREADY_INITIALIZED, Logger_Create(0));
}

void test_Deinitialization (void)
{
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Destroy());
    TEST_ASSERT_EQUAL(LOGGER_ERROR_NOT_INITIALIZED, Logger_Destroy());
}

void test_InitializationOfDependencies (void)
{
    TEST_ASSERT_TRUE(FakeTransmitter_GetInitialized());
}

void test_LogFilteredMessage (void)
{
    TEST_ASSERT_EQUAL(LOGGER_ERROR_MESSAGE_FILTERED, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_TRACE, 10, 0, 0));
}

void test_LogEnabledMessage (void)
{
    uint8_t payload[] = {};
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload));
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload)));

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_InstantMessagesOnlyInEnabledModes (void)
{
    setLoggerMode(LOGGER_MODE_BUFFERED);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    FakeTransmitter_GetTransmitBuffer(TRANSMITTER_CALLBACK_GROUP_INSTANT,&messagesTransmitted, &messagesTransmittedSize);
    TEST_ASSERT_EQUAL_INT(expectedMessageIndex, messagesTransmittedSize);

    setLoggerMode(LOGGER_MODE_INSTANT);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);

    tearDown();setUp();
    setLoggerMode(LOGGER_MODE_MIXED);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_DeinitializationClearsFilter (void)
{
    uint8_t payload[] = {};
    Logger_Destroy();
    Logger_Create(LOGGER_MODE_INSTANT);
    TEST_ASSERT_EQUAL(LOGGER_ERROR_MESSAGE_FILTERED, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload)));
}

void test_SetFilterEnablingLowerLevelMessageFromSingleSubsystemAndLevel (void)
{
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, true, false);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_SetFilterInclusiveEnableAllLowerLevels (void)
{
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);

    Logger_SetFilter(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, true, true);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_SetFilterEnableLevelForAllSubsystems (void)
{
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    appendExpectedMessage(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_EVENT, true, false);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_SetFilterEnableInclusiveForAllSystems (void)
{
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    appendExpectedMessage(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    appendExpectedMessage(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);

    Logger_SetFilter(LOG_SUBSYSTEM_COUNT, LOG_LEVEL_DATA, true, true);

    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_EVENT, 10, 0, 0);
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_DATA, 11, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_EVENT, 12, 0, 0);
    Logger_Log(LOG_SUBSYS_SYSTEM, LOG_LEVEL_DATA, 13, 0, 0);

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_LogMessageWithPayload (void)
{
    uint8_t payload[] = {13, 23, 43, 53};
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload));
    Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload));

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
}

void test_DelayedMessageFilteredAndKept (void)
{
    setLoggerMode(LOGGER_MODE_BUFFERED);
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0));
    TEST_ASSERT_EQUAL(LOGGER_ERROR_MESSAGE_FILTERED, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_TRACE, 10, 0, 0));
}

void test_DelayedMessageSentAndFlushed (void)
{
    setLoggerMode(LOGGER_MODE_BUFFERED);
    
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0);
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0));

    FakeTransmitter_GetTransmitBuffer(TRANSMITTER_CALLBACK_GROUP_DELAYED,&messagesTransmitted, &messagesTransmittedSize);
    TEST_ASSERT_EQUAL(0, messagesTransmittedSize);

    Logger_Flush();
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);
}

void test_DelayedMessagesOnlyInEnabledModes (void)
{
    setLoggerMode(LOGGER_MODE_INSTANT);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    Logger_Flush();
    FakeTransmitter_GetTransmitBuffer(TRANSMITTER_CALLBACK_GROUP_DELAYED,&messagesTransmitted, &messagesTransmittedSize);
    TEST_ASSERT_EQUAL_INT(expectedMessageIndex, messagesTransmittedSize);

    setLoggerMode(LOGGER_MODE_BUFFERED);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    Logger_Flush();
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);

    tearDown();setUp();
    setLoggerMode(LOGGER_MODE_MIXED);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0);
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0,0));
    Logger_Flush();
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);
}

void test_DelayedMessagesWithPayload (void)
{
    setLoggerMode(LOGGER_MODE_BUFFERED);
    uint8_t payload1[] = {10,20,30};
    uint8_t payload2[] = {40,50,60};

    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload1, sizeof(payload1));
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload1, sizeof(payload1)));
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 11, payload2, sizeof(payload2));
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 11, payload2, sizeof(payload2)));

    Logger_Flush();
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);
}

void test_DelayedMessageBufferOverflow (void)
{
    uint8_t payload[LOGGER_MAX_BUFFER_SIZE-LOGGER_MESSAGE_MIN_LENGHT]; 
    size_t index = 0;
    while (index < (LOGGER_MAX_BUFFER_SIZE-LOGGER_MESSAGE_MIN_LENGHT))
    {
        payload[index] = index;
        index++;
    }

    setLoggerMode(LOGGER_MODE_BUFFERED);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload));
    /* Payload fills available buffer space */
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload)));

    /* Repeat message, buffer overflow */
    TEST_ASSERT_EQUAL(LOGGER_ERROR_MESSAGE_BUFFER_FULL,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, payload, sizeof(payload)));
    /* Flush full buffer */
    TEST_ASSERT_EQUAL(LOGGER_ERROR_MESSAGE_BUFFER_FULL, Logger_Flush());
    /* Flushing empty buffer should return OK*/
    TEST_ASSERT_EQUAL(LOGGER_OK, Logger_Flush());
    /* Check flushed message */
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);
}

void test_MixedMessagesSent (void)
{
    setLoggerMode(LOGGER_MODE_MIXED);
    appendExpectedMessage(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0);
    TEST_ASSERT_EQUAL(LOGGER_OK,Logger_Log(LOG_SUBSYS_LOGGER, LOG_LEVEL_ERROR, 10, 0, 0));
    Logger_Flush();

    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_INSTANT);
    checkTransmittedMessages(TRANSMITTER_CALLBACK_GROUP_DELAYED);
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_CallingInterfaceWithoutInitializationReturnsError);
    RUN_TEST(test_FirstAndNextInitializations);
    RUN_TEST(test_Deinitialization);
    RUN_TEST(test_InitializationOfDependencies);

    RUN_TEST(test_LogFilteredMessage);
    RUN_TEST(test_LogEnabledMessage);
    RUN_TEST(test_InstantMessagesOnlyInEnabledModes);
    RUN_TEST(test_DeinitializationClearsFilter);
    RUN_TEST(test_SetFilterEnablingLowerLevelMessageFromSingleSubsystemAndLevel);
    RUN_TEST(test_SetFilterInclusiveEnableAllLowerLevels);
    RUN_TEST(test_SetFilterEnableLevelForAllSubsystems);
    RUN_TEST(test_SetFilterEnableInclusiveForAllSystems);
    RUN_TEST(test_LogMessageWithPayload);

    RUN_TEST(test_DelayedMessageFilteredAndKept);
    RUN_TEST(test_DelayedMessageSentAndFlushed);
    RUN_TEST(test_DelayedMessagesOnlyInEnabledModes);
    RUN_TEST(test_DelayedMessagesWithPayload);
    RUN_TEST(test_DelayedMessageBufferOverflow);

    RUN_TEST(test_MixedMessagesSent);

    UNITY_END();
}