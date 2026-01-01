/* Lista de testes do Transmitter
/* Objetivo:    Fornecer flexibilidade nos canais de transmissão usados pelo logger
/* Descrição:   Registra funções de callback e envia mensagens para essas assim que as recebe, semelhante ao padrão Observer
 *              Possível ser extendido para remover funções de callback
 * 
 * Registrar callback retorna verdadeiro                                                                OK
 * Enviar mensagem chama callBack com mensagem enviada                                                  OK
 * Destruir módulo remove todos os callbacks                                                            OK
 * É possível registrar o mesmo callback múltiplas vezes                                                OK
 * Tentar registrar mais callbacks que o máximo retorna falso                                           OK
 * Enviar mensagem chama todos os callbacks com mensagem enviada
 * 
 * 
 * 
*/

#include <unity.h>
#include <Transmitter/include/Transmitter.h>

static int callbackVar = 0;
static uint8_t * storedPayload;
static size_t  storedPayloadSize = 0;

static void testCallback1 (uint8_t * payload, size_t payloadSize)
{
    callbackVar++;
    storedPayload = payload;
    storedPayloadSize = payloadSize;
}

void setUp(void)
{
    /* Reiniciar variáveis de teste */
    callbackVar = 0;
    storedPayload = 0;
    storedPayloadSize = 0;
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

void test_AttachCallback (void)
{
    TEST_ASSERT_TRUE(Transmitter_AttachTransmitCallback(testCallback1));
}

void test_CallAttachedCallback (void)
{
    uint8_t testPayload[] = {0,1,2,3,4};

    Transmitter_AttachTransmitCallback(testCallback1);
    Transmitter_TransmitPayload(testPayload, sizeof(testPayload));

    TEST_ASSERT_EQUAL(testPayload, storedPayload);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(testPayload, storedPayload, sizeof(testPayload));
}

void test_DestroyModuleErasesAttachedCallbacks (void)
{
    uint8_t testPayload[] = {0,1,2,3,4};

    Transmitter_AttachTransmitCallback(testCallback1);
    Transmitter_Destroy();
    Transmitter_TransmitPayload(testPayload, sizeof(testPayload));

    TEST_ASSERT_FALSE(storedPayload);
}

void test_AttachSameCallbackMultipleTimes (void)
{
    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        TEST_ASSERT_TRUE(Transmitter_AttachTransmitCallback(testCallback1));
    }
}

void test_AttachMoreThanCallbackCapacityReturnsFalse (void)
{
    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        TEST_ASSERT_TRUE(Transmitter_AttachTransmitCallback(testCallback1));
    }

    TEST_ASSERT_FALSE(Transmitter_AttachTransmitCallback(testCallback1));
}

void test_CallAllCallbacksOnMessageReceived (void)
{
    uint8_t testPayload[] = {0,1,2,3,4};

    for (int i = 0; i < TRANSMITTER_MAX_ATTACHED_CALLBACKS; i++)
    {
        Transmitter_AttachTransmitCallback(testCallback1);
    }
    Transmitter_TransmitPayload(testPayload, sizeof(testPayload));
    
    TEST_ASSERT_EQUAL_INT(TRANSMITTER_MAX_ATTACHED_CALLBACKS, callbackVar);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_AttachCallback);
    RUN_TEST(test_CallAttachedCallback);
    RUN_TEST(test_DestroyModuleErasesAttachedCallbacks);
    RUN_TEST(test_AttachSameCallbackMultipleTimes);
    RUN_TEST(test_AttachMoreThanCallbackCapacityReturnsFalse);
    RUN_TEST(test_CallAllCallbacksOnMessageReceived);

    UNITY_END();
}