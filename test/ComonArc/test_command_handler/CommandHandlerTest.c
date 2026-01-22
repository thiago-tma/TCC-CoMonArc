/*
 * Lista de testes do Command Handler
 * Objetivo: Identificar e executar comandos através da análise de strings, a partir de uma tabela de comandos
 * Descrição: O módulo recebe uma string terminada por \n, separa em argumentos divididos por espaços e procura na tabela o nome do comando, o primeiro argumento
 * 
 * Inicialização do módulo                                                              OK
 * Deinicialização do módulo                                                            OK
 * Tentativa de processar comando sem inicialização retorna erro                        OK            
 * Comando inexistente deve retornar erro                                               OK
 * String de comando vazia deve retornar erro                                           OK
 * Comando existente (sem argumentos) é executado                                       OK
 * Comando com argumentos de texto corretamente reconhecidos                            OK
 * Comando com argumentos de inteiros devidamente reconhecidos
 * 
*/

#include <unity.h>
#include <CommandHandler/include/CommandHandler.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

uint32_t testVar = 0;

bool parse_long(const char *s, long min, long max, long *out)
{
    char *end;

    /*errno = 0;*/
    long v = strtol(s, &end, 0);  // base 0: auto-detect 0x, 0, decimal

    /* 1. No digits were found */
    if (end == s)
        return false;

    /* 2. Trailing non-numeric characters */
    if (*end != '\0')
        return false;

    /* 3. Overflow or underflow */
    if (/*(errno == ERANGE) ||*/ v < min || v > max)
        return false;

    *out = v;
    return true;
}

static void testFunction1 (int argc, char **argv)
{
    testVar++;
}

static void testFunction2 (int argc, char **argv)
{
    if (argc != 3) return;
    
    for (int i = 1; i < 3; i++)
    {
        if (strcmp(argv[i], "text1") == 0) testVar += 50;
        if (strcmp(argv[i], "text2") == 0) testVar += 100;
    }
}

static void testFunction3 (int argc, char **argv)
{
    long convertedValue;
    if (argc != 2) return;

    if (!parse_long(argv[1], -100, 100, &convertedValue)) return;
    testVar += convertedValue;
}

const CommHandler_Command_t testCommandTable[] =
{
    /*   Name       CommandFunction           */
    {"Ping1",       testFunction1},
    {"Ping2",       testFunction2},
    {"Ping3",       testFunction3},
    {"", 0 }
};

void setUp (void)
{
    testVar = 0;

    CommandHandler_Create(testCommandTable);
}

void tearDown (void)
{

}

void test_HandlerInitialization (void)
{
    TEST_ASSERT_EQUAL(COMMHANDLER_ERROR_NO_COMMAND_TABLE,CommandHandler_Create(0));
    TEST_ASSERT_EQUAL(COMMHANDLER_OK,CommandHandler_Create(testCommandTable));
}

void test_HandlerDeinitialization (void)
{
    TEST_ASSERT_EQUAL(COMMHANDLER_OK, CommandHandler_Destroy());
    TEST_ASSERT_EQUAL(COMMHANDLER_ERROR_NOT_INITIALIZED, CommandHandler_Destroy());
}

void test_CommandWithNoInitialization (void)
{
    char commandString[] = "Ping1\n";

    CommandHandler_Destroy();
    TEST_ASSERT_EQUAL(COMMHANDLER_ERROR_NOT_INITIALIZED, CommandHandler_ProcessCommand(commandString, sizeof(commandString)));

}

void test_ErrorWhenNoMatchingCommandFound (void)
{
    char commandString[] = "InexistentCommand\n";

    TEST_ASSERT_EQUAL(COMMHANDLER_ERROR_NO_COMMAND_FOUND, CommandHandler_ProcessCommand(commandString, sizeof(commandString)));
}

void test_ErrorWhenEmptyCommandStringPassed (void)
{
    char commandString[] = "";

    TEST_ASSERT_EQUAL(COMMHANDLER_ERROR_NO_STRING, CommandHandler_ProcessCommand(commandString, sizeof(commandString)));
}

void test_CommandWithNoArguments (void)
{
    char commandString[] = "Ping1\n";
    
    TEST_ASSERT_EQUAL(COMMHANDLER_OK, CommandHandler_ProcessCommand(commandString, sizeof(commandString)));
    TEST_ASSERT_EQUAL(1, testVar);
}

void test_CommandWithTextArguments (void)
{
    char commandString2[] = "Ping2 fake text1\n";
    char commandString3[] = "Ping2 text2 fake\n";

    TEST_ASSERT_EQUAL(COMMHANDLER_OK, CommandHandler_ProcessCommand(commandString2, sizeof(commandString2)));
    TEST_ASSERT_EQUAL(50, testVar);
    TEST_ASSERT_EQUAL(COMMHANDLER_OK, CommandHandler_ProcessCommand(commandString3, sizeof(commandString3)));
    TEST_ASSERT_EQUAL(150, testVar);
}

void test_CommandWithNumberArguments (void)
{
    char commandString4[] = "Ping3 83";

    TEST_ASSERT_EQUAL(COMMHANDLER_OK, CommandHandler_ProcessCommand(commandString4, sizeof(commandString4)));
    TEST_ASSERT_EQUAL(83, testVar);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_HandlerInitialization);
    RUN_TEST(test_HandlerDeinitialization);
    RUN_TEST(test_CommandWithNoInitialization);
    RUN_TEST(test_ErrorWhenNoMatchingCommandFound);
    RUN_TEST(test_ErrorWhenEmptyCommandStringPassed);
    RUN_TEST(test_CommandWithNoArguments);
    RUN_TEST(test_CommandWithTextArguments);
    RUN_TEST(test_CommandWithNumberArguments);

    UNITY_END();
}