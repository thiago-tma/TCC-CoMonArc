/* Lista de testes do Transmitter
/* Função:      Executar funções sobre as mensagens inseridas com base nas funções registradas. Objetivo principal seria a trasnmissão das mesmas
/* Descrição:   Registra funções de callback e envia mensagens para essas assim que as recebe, semelhante ao padrão Observer
 *              Possui dois grupos de callBack, instant e delayed.
 * 
 * Chamar funções sem inicialização retorna erro                                                OK                                                     
 * Inicialização dupla retorna aviso                                                            OK           
 * Deinicialização retorna OK                                                                   OK      
 * 
 * Registrar callback para um grupo retorna OK                                                  OK
 * Tentar registrar mais callbacks que o limite retorna erro                                    OK
 * Tentar registrar callback em outro grupo com o primeiro grupo cheio retorna OK               OK
 * Transmitir mensagens chama callback do grupo passado                                         OK                                             
 * Enviar mensagem chama callBack com mensagem enviada para um grupo                            OK
 * Enviar mensagem chama todo os callback do grupo (mais de 1)                                  OK                                         
 * Deinicialização limpa callbacks                                                              OK
 * 
 * 
*/

#include <unity.h>
#include <Transmitter/include/Transmitter.h>

static int callbackVar = 0;
static uint8_t storedMessageInstant[100];
static uint8_t storedMessageDelayed[100];
static size_t  storedMessageInstantSize = 0, storedMessageDelayedSize = 0;

static void testCallbackInstant (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    callbackVar++;

    int index = 0;
    storedMessageInstant[index++] = origin;
    storedMessageInstant[index++] = level;
    storedMessageInstant[index++] = messageID;

    int payloadIndex = 0;
    while(payloadIndex < payloadSize)
    {
        storedMessageInstant[index++] = payload[payloadIndex];
        payloadIndex++;
    }
    storedMessageInstant[index++] = payloadSize;
}

static void testCallbackDelayed (Log_Subsystem_t  origin, Log_Level_t level, Log_MessageId_t messageID, uint8_t * payload, size_t payloadSize)
{
    callbackVar += 10;

    int index = 0;
    storedMessageDelayed[index++] = origin;
    storedMessageDelayed[index++] = level;
    storedMessageDelayed[index++] = messageID;

    int payloadIndex = 0;
    while(payloadIndex < payloadSize)
    {
        storedMessageDelayed[index++] = payload[payloadIndex];
        payloadIndex++;
    }
    storedMessageDelayed[index++] = payloadSize;
}

void setUp(void)
{
    /* Reiniciar variáveis de teste */
    callbackVar = 0;
    Transmitter_Create();
}

void tearDown(void)
{
    Transmitter_Destroy();
}

void test_CreateAndDestroy(void)
{
    Transmitter_Create();
    Transmitter_Destroy();
}

void test_CheckInitialization (void)
{
    uint8_t testPayload[] = {0,1,2,3,4};
    Transmitter_Destroy();

    TEST_ASSERT_EQUAL_MESSAGE(TRANSMITTER_ERROR_NOT_INITIALIZED, Transmitter_Destroy(), "function 1");
    TEST_ASSERT_EQUAL_MESSAGE(TRANSMITTER_ERROR_NOT_INITIALIZED, Transmitter_AttachTransmitCallback(0,0), "function 2");
    TEST_ASSERT_EQUAL_MESSAGE(TRANSMITTER_ERROR_NOT_INITIALIZED, Transmitter_Transmit(0,0,0,0,0,0), "function 3");
}

void test_DoubleInitialization (void)
{
    Transmitter_Destroy();
    TEST_ASSERT_EQUAL(TRANSMITTER_OK, Transmitter_Create());
    TEST_ASSERT_EQUAL(TRANSMITTER_ERROR_INITIALIZED, Transmitter_Create());
}

void test_Deinitialization (void)
{
    TEST_ASSERT_EQUAL(TRANSMITTER_OK, Transmitter_Destroy());
    TEST_ASSERT_EQUAL(TRANSMITTER_ERROR_NOT_INITIALIZED, Transmitter_Destroy());
}

void test_AttachCallback (void)
{
    TEST_ASSERT_EQUAL(TRANSMITTER_OK ,Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT));
    TEST_ASSERT_EQUAL(TRANSMITTER_OK ,Transmitter_AttachTransmitCallback(testCallbackDelayed, TRANSMITTER_CALLBACK_GROUP_DELAYED));
}

void test_AttachCallbackCapacityReached (void)
{
    int index = 0;
    while (index < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP)
    {
        Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
        index++;
    }
    TEST_ASSERT_EQUAL(TRANSMITTER_ERROR_CALLBACK_CAPACITY_FULL, Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT));
}

void test_AttachCallbackCapacityInOtherGroup (void)
{
    int index = 0;
    while (index < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP)
    {
        Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
        index++;
    }
    TEST_ASSERT_EQUAL(TRANSMITTER_OK, Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_DELAYED));
}

void test_CallCallbacksInEachGroup (void)
{
    Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
    Transmitter_AttachTransmitCallback(testCallbackDelayed, TRANSMITTER_CALLBACK_GROUP_DELAYED);

    TEST_ASSERT_EQUAL(TRANSMITTER_OK ,Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_INSTANT, 0,0,0,0,0));
    TEST_ASSERT_EQUAL(1, callbackVar);
    TEST_ASSERT_EQUAL(TRANSMITTER_OK ,Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_DELAYED, 0,0,0,0,0));
    TEST_ASSERT_EQUAL(11, callbackVar);
}

void test_CallbackReceivesMessageAndParameters (void)
{
    uint8_t payload[] = {10,20,30};
    uint8_t expectedMessage[] = {0, 1, 2, 10, 20, 30, 3};

    Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
    Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_INSTANT, 0, 1, 2, payload, sizeof(payload));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedMessage, storedMessageInstant, sizeof(expectedMessage));
}

void test_CallAllAttachedCallbacks (void)
{
    int index = 0;
    while (index < TRANSMITTER_MAX_ATTACHED_CALLBACKS_PER_GROUP)
    {
        Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
        Transmitter_AttachTransmitCallback(testCallbackDelayed, TRANSMITTER_CALLBACK_GROUP_DELAYED);
        index++;
    }

    Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_INSTANT, 0,0,0,0,0);
    TEST_ASSERT_EQUAL(3, callbackVar);
    Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_DELAYED, 0,0,0,0,0);
    TEST_ASSERT_EQUAL(33, callbackVar);
}

void test_DeinitializationClearsCallbacks (void)
{
    Transmitter_AttachTransmitCallback(testCallbackInstant, TRANSMITTER_CALLBACK_GROUP_INSTANT);
    Transmitter_Destroy();
    Transmitter_Create();
    Transmitter_Transmit(TRANSMITTER_CALLBACK_GROUP_INSTANT, 0,0,0,0,0);
    TEST_ASSERT_EQUAL(0, callbackVar);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_CheckInitialization);
    RUN_TEST(test_DoubleInitialization);
    RUN_TEST(test_Deinitialization);
    RUN_TEST(test_AttachCallback);
    RUN_TEST(test_AttachCallbackCapacityReached);
    RUN_TEST(test_AttachCallbackCapacityInOtherGroup);
    RUN_TEST(test_CallCallbacksInEachGroup);
    RUN_TEST(test_CallbackReceivesMessageAndParameters);
    RUN_TEST(test_CallAllAttachedCallbacks);
    RUN_TEST(test_DeinitializationClearsCallbacks);

    UNITY_END();
}