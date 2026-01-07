/*
 * Lista de testes do Receiver
 * Objetivo: ofercer uma interface capaz de se adaptar a plataforma presente e identificar streams completas de comando
 * Descrição: O módulo oferece duas formas de receber a stream, armazena streams incompletas por um período antes do
 *      descarte e encaminha streams completas (que são terminas pelo caractere \n) para o Command Handler processar
 * 
 * Inicialização sem operação de callback de comando                                        OK
 *      Erro de falta de callback de timer                                                  OK
 * Inicialização com operação de callback de comando                                        OK
 *      Erro de falta de callback de timer                                                  OK
 *      Erro de falta de callback de comando                                                OK
 * Deinicialização correta                                                                  OK
 *      Retorna OK se já inicializado                                                       OK
 *      Retorna erro se já deinicializado                                                   OK
 * Funções de manutenção e entrega de dados retornam erro sem inicialização do módulo       OK
 * Operação sem callback
 *      Entrega de dados retorna OK ao armazenar dados                                      OK
 *      Entrega de dados retorna Erro ao estourar buffer de armazenamento                   OK
 *      Entregar string com terminação '\n' chama função de command handler com a string    OK
 *      Chamar commandHandler sem inicialização do command handler resulta em erro          OK
 *      Chamar commandHandler com string sem comando reconhecido resulta em erro
 *      Chamar commandHandler retornando erro desconnhecido resulta em erro
 *      Entregar string sem terminação '\n' mantém string armazenada                        
 *      Entrega de string com terminação '\n' em duas chamadas separadas
 *      Destruir módulo descarta caracters armazenados
 *      Timeout após tempo limite descarta string armazenada
 *      Adição de novos caracteres renova timer de timeout
 * Operação com callback de comando
 * 
*/

#include <unity.h>
#include <Receiver/include/Receiver.h>
#include "FakeCommandHandler.h"

static timeMicroseconds systemTime = 0;
static char storedMessage[RECEIVER_MAX_COMMAND_BUFFER_SIZE];
static size_t storedMessageSize = 0;
static size_t receivedMaxMessageSize = 0;

static timeMicroseconds testSystemTimeCallback (void)
{
    return systemTime;
}

static void testCommandCallback (size_t messageMaxSize, char * message, size_t * messageSize)
{
    size_t setMessageSize = (messageMaxSize > storedMessageSize ? storedMessageSize : messageMaxSize);
    receivedMaxMessageSize = messageMaxSize;

    for (size_t i = 0; i < setMessageSize; i++)
    {
        message[i] = storedMessage[i];
    }

    *messageSize = setMessageSize;
}

void setUp (void)
{
    systemTime = 0;
    storedMessageSize = 0;
    receivedMaxMessageSize = 0;

    FakeCommandHandler_Reset();
    Receiver_Create(false, 0, testSystemTimeCallback);
}

void tearDown (void)
{
    Receiver_Destroy();
}

void test_InitializationWithoutCommandCallback (void)
{
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NO_TIMER_CALLBACK, Receiver_Create(false, 0, 0));
    TEST_ASSERT_EQUAL(RECEIVER_OK,  Receiver_Create(false, 0, testSystemTimeCallback));
}

void test_InitializationWithCommandCallback (void)
{
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NO_TIMER_CALLBACK, Receiver_Create(true, 0, 0));
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NO_COMMAND_CALLBACK,  Receiver_Create(true, 0, testSystemTimeCallback));
    TEST_ASSERT_EQUAL(RECEIVER_OK,  Receiver_Create(true, testCommandCallback, testSystemTimeCallback));
}

void test_Deinitialization (void)
{
    TEST_ASSERT_EQUAL(RECEIVER_OK,  Receiver_Destroy());
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NOT_INITIALIZED,  Receiver_Destroy());
}

void test_CallingFunctionsWithoutInitializationReturnsError (void)
{
    char testMessage[RECEIVER_MAX_COMMAND_BUFFER_SIZE];
    size_t receivedBytes;

    Receiver_Destroy();
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NOT_INITIALIZED, Receiver_Run());
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_NOT_INITIALIZED, Receiver_ReceiveMessage(testMessage, RECEIVER_MAX_COMMAND_BUFFER_SIZE, &receivedBytes));
}

void test_SendingCharactersReturnsOKOnReception (void)
{
    char testMessage[RECEIVER_MAX_COMMAND_BUFFER_SIZE] = {"This is a test message\n"};
    size_t receivedBytes;

    TEST_ASSERT_EQUAL(RECEIVER_OK, Receiver_ReceiveMessage(testMessage, RECEIVER_MAX_COMMAND_BUFFER_SIZE, &receivedBytes));
}

void test_OverflowOfInternalBufferOnSendingCharactersReturnsError (void)
{
    char testMessage[RECEIVER_MAX_COMMAND_BUFFER_SIZE] = {"This is a test message\n"};
    size_t receivedBytes;

    TEST_ASSERT_EQUAL(RECEIVER_OK, Receiver_ReceiveMessage(testMessage, RECEIVER_MAX_COMMAND_BUFFER_SIZE/2, &receivedBytes));
    TEST_ASSERT_EQUAL(RECEIVER_MAX_COMMAND_BUFFER_SIZE/2, receivedBytes);
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_BUFFER_CAPACITY_REACHED, Receiver_ReceiveMessage(testMessage, RECEIVER_MAX_COMMAND_BUFFER_SIZE, &receivedBytes));
    TEST_ASSERT_EQUAL(RECEIVER_MAX_COMMAND_BUFFER_SIZE - RECEIVER_MAX_COMMAND_BUFFER_SIZE/2, receivedBytes);
}

void test_ReceiverSendsFinishedMessageToCommandHandler (void)
{
    size_t receivedBytes, commHandlerBytes;
    char * commHandlerMessage;
    char testMessage[] = {"This is a test message\n"};

    TEST_ASSERT_EQUAL_MESSAGE(RECEIVER_OK, Receiver_ReceiveMessage(testMessage, sizeof(testMessage), &receivedBytes), "Receive Message not OK");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(testMessage), receivedBytes, "Message received by Receiver wrong");
    Receiver_Run();
    FakeCommandHandler_GetSentString(&commHandlerMessage, &commHandlerBytes);
    /*sizeof(testMessage)-1, como o caractere de final de string '\0' não é contabilizado*/
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(testMessage)-1, commHandlerBytes, "Number of bytes received by CommandHalnder wrong");     
    TEST_ASSERT_EQUAL_CHAR_ARRAY_MESSAGE(testMessage, commHandlerMessage, commHandlerBytes, "Message received by CommandHAndler wrong");
    
}

void test_CallingCommandHandleWithNoInitializationReturnsError (void)
{
    size_t receivedBytes;
    char testMessage[] = {"This is a test message\n"};

    Receiver_ReceiveMessage(testMessage, sizeof(testMessage), &receivedBytes);
    FakeCommandHandler_SetInitialized(false);
    TEST_ASSERT_EQUAL(RECEIVER_ERROR_COMMANDHANDLER_NOT_INITIALIZED, Receiver_Run());   
}

/*
void test_ReceiverKeepsUnfinishedMessages (void)
{
    size_t receivedBytes;
    char testMessage1[] = {"This is a test "};
    char testMessage2[] = {"message"};


}
*/

int main (int argc, char ** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_InitializationWithoutCommandCallback);
    RUN_TEST(test_InitializationWithCommandCallback);
    RUN_TEST(test_Deinitialization);
    RUN_TEST(test_CallingFunctionsWithoutInitializationReturnsError);
    RUN_TEST(test_SendingCharactersReturnsOKOnReception);
    RUN_TEST(test_OverflowOfInternalBufferOnSendingCharactersReturnsError);
    RUN_TEST(test_ReceiverSendsFinishedMessageToCommandHandler);
    RUN_TEST(test_CallingCommandHandleWithNoInitializationReturnsError);

    UNITY_END();
}