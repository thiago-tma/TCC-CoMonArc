/* Lista de testes do módulo CurrentSensor
 * Objetivo: Disponibilizar a leitura do sensor de corrente (resistivo) para o restante do sistema
 * Descrição: Sensor pede a leitura do ADC por uma função, traduz a leitura para tensão e depois para corrente.
 *      Salva a nova leitura na memória para entregar para outros módulos que desejem a leitura.
 * 
 * Inicialização retorna OK, inicializar novamente retorna erro                         OK
 * Deinicialização pós inicialização retorna OK, repetir retorna erro                   OK
 * Chamar outras funções sem inicialização retorna erro;                                OK
 * 
 * Inicialização inicializa dependências (CS_ADC)                                       OK
 * Deinicialização deinicializa dependências (CS_ADC)                                   OK
 * 
 * Consulta sem realizar a primeira leitura retorna erro e 0 como leitura.              OK
 * Receber parâmetros nulos do ADC retorna erro na inicialização                        OK
 * Conversão correta de ADC dado os parâmetros e a leitura recebida
 *      100ohms de resistência, 5V de referência e 10bits de resolução (1023max)        OK
 *      47ohms  de resistência, 5V de referência e 10bits de resolução (1023max)        OK
 *      100ohms de resistência, 1V de referência e 12bits de resolução (4095max)        OK
 * Conversão correta duas vezes seguidas, com valores diferentes de leitura             OK
 */

#include <unity.h>
#include <CurrentSensor.h>
#include "FakeCS_ADC.h"

static void setNewADC (CS_ADC_Parameters_t parameters)
{
    CurrentSensor_Destroy();
    FakeCS_ADC_SetParameters(parameters);
    CurrentSensor_Create();
}

void setUp (void)
{
    FakeCS_ADC_SetParameters((CS_ADC_Parameters_t){.adcMaxValue=1023,.referenceVoltageMillivolts=5000, .shuntResistanceMilliohms = 100000});
    CurrentSensor_Create();
}

void tearDown (void)
{
    CurrentSensor_Destroy();
}   

void test_Initialization (void)
{
    CurrentSensor_Destroy();
    TEST_ASSERT_EQUAL(CURRENTSENSOR_OK,CurrentSensor_Create());
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_ALREADY_INITIALIZED,CurrentSensor_Create());
}

void test_Deinitialization (void)
{
    TEST_ASSERT_EQUAL(CURRENTSENSOR_OK,CurrentSensor_Destroy());
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_NOT_INITIALIZED,CurrentSensor_Destroy());
}

void test_UsingTheModuleWithoutInitializationReturnsError (void)
{
    CurrentSensor_Destroy();
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_NOT_INITIALIZED,CurrentSensor_GetValue(0));
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_NOT_INITIALIZED,CurrentSensor_NewRead());
}

void test_InitializeAndDeinitializeDependecies (void)
{
    TEST_ASSERT_TRUE(FakeCS_ADC_GetInitialized());
    CurrentSensor_Destroy();
    TEST_ASSERT_FALSE(FakeCS_ADC_GetInitialized());
}

void test_GetCurrentWithoutFirstReadReturnsError (void)
{
    Current_Microamps_t reading;
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_NO_AVAILABLE_READING, CurrentSensor_GetValue(&reading));
}

void test_NullADCParametersReturnsError (void)
{
    setNewADC((CS_ADC_Parameters_t){.adcMaxValue=0,.referenceVoltageMillivolts=5000, .shuntResistanceMilliohms = 100});
    CurrentSensor_Destroy();
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_ADC_NULL_PARAMETER, CurrentSensor_Create());

    setNewADC((CS_ADC_Parameters_t){.adcMaxValue=1023,.referenceVoltageMillivolts=0, .shuntResistanceMilliohms = 100});
    CurrentSensor_Destroy();
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_ADC_NULL_PARAMETER, CurrentSensor_Create());

    setNewADC((CS_ADC_Parameters_t){.adcMaxValue=1023,.referenceVoltageMillivolts=5000, .shuntResistanceMilliohms = 0});
    CurrentSensor_Destroy();
    TEST_ASSERT_EQUAL(CURRENTSENSOR_ERROR_ADC_NULL_PARAMETER, CurrentSensor_Create());
}

void test_NewReadOnADCType1 (void)
{
    Current_Microamps_t current;
    int32_t resistanceMilliohms = 100000, referenceVoltageMillivolts=5000, adcMax = 1023;
    int64_t numerator, denominator, conversionFactor;
    CS_ADC_Parameters_t testParameters = (CS_ADC_Parameters_t){.adcMaxValue=adcMax, .referenceVoltageMillivolts = referenceVoltageMillivolts, .shuntResistanceMilliohms = resistanceMilliohms};

    numerator = (int64_t)referenceVoltageMillivolts*1000000;
    denominator = (int64_t)adcMax*resistanceMilliohms;
    conversionFactor = (numerator/denominator);

    setNewADC(testParameters);
    FakeCS_ADC_SetRead(500);
    CurrentSensor_NewRead();
    CurrentSensor_GetValue(&current);
    TEST_ASSERT_EQUAL_MESSAGE((int32_t)conversionFactor*500, current, "Read with ADC failed (1)");
}

void test_NewReadOnADCType2 (void)
{
    Current_Microamps_t current;
    int32_t resistanceMilliohms = 47000, referenceVoltageMillivolts=5000, adcMax = 1023;
    int64_t numerator, denominator, conversionFactor;
    CS_ADC_Parameters_t testParameters = (CS_ADC_Parameters_t){.adcMaxValue=adcMax, .referenceVoltageMillivolts = referenceVoltageMillivolts, .shuntResistanceMilliohms = resistanceMilliohms};

    numerator = (int64_t)referenceVoltageMillivolts*1000000;
    denominator = (int64_t)adcMax*resistanceMilliohms;
    conversionFactor = (numerator/denominator);

    setNewADC(testParameters);
    FakeCS_ADC_SetRead(700);
    CurrentSensor_NewRead();
    CurrentSensor_GetValue(&current);
    TEST_ASSERT_EQUAL_MESSAGE((int32_t)conversionFactor*700, current, "Read with ADC failed (2)");
}

void test_NewReadOnADCType3 (void)
{
    Current_Microamps_t current;
    int32_t resistanceMilliohms = 100000, referenceVoltageMillivolts=1000, adcMax = 4095;
    int64_t numerator, denominator, conversionFactor;
    CS_ADC_Parameters_t testParameters = (CS_ADC_Parameters_t){.adcMaxValue=adcMax, .referenceVoltageMillivolts = referenceVoltageMillivolts, .shuntResistanceMilliohms = resistanceMilliohms};

    numerator = (int64_t)referenceVoltageMillivolts*1000000;
    denominator = (int64_t)adcMax*resistanceMilliohms;
    conversionFactor = (numerator/denominator);

    setNewADC(testParameters);
    FakeCS_ADC_SetRead(2500);
    CurrentSensor_NewRead();
    CurrentSensor_GetValue(&current);
    TEST_ASSERT_EQUAL_MESSAGE((int32_t)conversionFactor*2500, current, "Read with ADC failed (3)");
}

void test_NewReadCorrectConversionTwice (void)
{
    Current_Microamps_t current;
    int32_t resistanceMilliohms = 100000, referenceVoltageMillivolts=5000, adcMax = 1023;
    int64_t numerator, denominator, conversionFactor;
    CS_ADC_Parameters_t testParameters = (CS_ADC_Parameters_t){.adcMaxValue=adcMax, .referenceVoltageMillivolts = referenceVoltageMillivolts, .shuntResistanceMilliohms = resistanceMilliohms};

    numerator = (int64_t)referenceVoltageMillivolts*1000000;
    denominator = (int64_t)adcMax*resistanceMilliohms;
    conversionFactor = (numerator/denominator);

    setNewADC(testParameters);
    FakeCS_ADC_SetRead(500);
    CurrentSensor_NewRead();
    CurrentSensor_GetValue(&current);
    TEST_ASSERT_EQUAL_MESSAGE((int32_t)conversionFactor*500, current, "Read with ADC failed (1)");

    FakeCS_ADC_SetRead(333);
    CurrentSensor_NewRead();
    CurrentSensor_GetValue(&current);
    TEST_ASSERT_EQUAL_MESSAGE((int32_t)conversionFactor*333, current, "Read with ADC failed (1)");
}

int main (int argc, char ** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Initialization);
    RUN_TEST(test_Deinitialization);
    RUN_TEST(test_UsingTheModuleWithoutInitializationReturnsError);
    RUN_TEST(test_InitializeAndDeinitializeDependecies);
    RUN_TEST(test_GetCurrentWithoutFirstReadReturnsError);
    RUN_TEST(test_NullADCParametersReturnsError);
    RUN_TEST(test_NewReadOnADCType1);
    RUN_TEST(test_NewReadOnADCType2);
    RUN_TEST(test_NewReadOnADCType3);
    RUN_TEST(test_NewReadCorrectConversionTwice);

    UNITY_END();
}