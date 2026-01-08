/*
 *  Lista de testes do YawController
 *  Objetivo: Controlar o servo motor para manter a guinada indicada pelo Magnetometro constante
 *  Descriçâo: Inicia (e reinicia) com haste em 90º. Após um intervalo definido, toma a leitura atual do sensor como referência
 *      e conserva a direção dentro dos limites de atuação da haste.
 * 
 * Erro ao chamar funções sem inicialização                                                 OK
 * Inicialização retorna OK                                                                 OK
 * Erro ao inicializar novamente, sem efeito no programa                                    OK
 * Deinicialização correta                                                                  OK
 * 
 * Inicialização inicializa módulo de controle do servo                                     OK
 * Inicialização define direção do servo em 90º                                             OK
 * Run() toma sensor como referência após timeout                                           OK
 *      Run() consequente corrige direção                                                   OK
 * Reset() realinha direção em 90º
 *      Run() pós timeout toma nova leitura de sensor como referência
 *      Run() em sequência corrige direção
 * Controle de servo é limitado entre 0 a 180 mesmo passando do limite
 * Controle de servo vai para outro extremo quando referência passa por '270º'
 * 
 * 
*/

#include <unity.h>
#include <YawController.h>
#include "FakeServoController.h"
#include "FakeSystemTimer.h"
#include "FakeMagnetometer.h"
#include <SystemClock.h>

void setUp (void)
{
    SystemClock_Create();
    YawController_Create();
}

void tearDown (void)
{
    SystemClock_Destroy();
    YawController_Destroy();
    FakeServoController_Reset();
    FakeMagnetometer_Reset();
}

void test_NoInitializationErrorInFunctions (void)
{
    YawController_Destroy();
    TEST_ASSERT_EQUAL(YAWCONTROLLER_ERROR_NOT_INITIALIZED, YawController_Destroy());
    TEST_ASSERT_EQUAL(YAWCONTROLLER_ERROR_NOT_INITIALIZED, YawController_Run());
    TEST_ASSERT_EQUAL(YAWCONTROLLER_ERROR_NOT_INITIALIZED, YawController_Reset());
}

void test_InitializationOK (void)
{
    YawController_Destroy();
    TEST_ASSERT_EQUAL(YAWCONTROLLER_OK, YawController_Create());
}

void test_InitializingTwiceHasNoEffectAndReturnsError (void)
{
    TEST_ASSERT_EQUAL(YAWCONTROLLER_ERROR_ALREADY_INITIALIZED, YawController_Create());
}

void test_DeinitializationReturnsOkOnlyWhenModuleIsAlreadyInitialized (void)
{
    TEST_ASSERT_EQUAL(YAWCONTROLLER_OK, YawController_Destroy());
}

void test_InitializationInitializesServoController (void)
{
    TEST_ASSERT_TRUE(FakeServoController_IsInitialized());
}

void test_InitializationInitializesMagnetometer (void)
{
    TEST_ASSERT_TRUE(FakeMagnetometer_IsInitialized());
}

void test_InitializationSetsServoOn90Degrees (void)
{
    TEST_ASSERT_EQUAL(90, FakeServoController_ReadHeading());
}

void test_GetReferenceAfterTimeoutAndCorrectHeading (void)
{
    FakeMagnetometer_SetReading(45);
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "Timer has not expired yet");

    FakeSystemTimer_AddTime(YAWCONTROLLER_RESET_TIME_MICROSECONDS+1);
    FakeMagnetometer_SetReading(60);
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "YawController must only get the reference signal");

    FakeSystemTimer_AddTime(YAWCONTROLLER_RESET_TIME_MICROSECONDS+1);
    FakeMagnetometer_SetReading(60-5);
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90+5, FakeServoController_ReadHeading(), "First yaw correction failed");

    FakeSystemTimer_AddTime(YAWCONTROLLER_RESET_TIME_MICROSECONDS+1);
    FakeMagnetometer_SetReading(60+5);
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90+5-5, FakeServoController_ReadHeading(),  "Second yaw correction failed");
}

int main (int argc, char ** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_NoInitializationErrorInFunctions);
    RUN_TEST(test_InitializationOK);
    RUN_TEST(test_InitializingTwiceHasNoEffectAndReturnsError);
    RUN_TEST(test_DeinitializationReturnsOkOnlyWhenModuleIsAlreadyInitialized);
    RUN_TEST(test_InitializationInitializesServoController);
    RUN_TEST(test_InitializationInitializesMagnetometer);
    RUN_TEST(test_InitializationSetsServoOn90Degrees);
    RUN_TEST(test_GetReferenceAfterTimeoutAndCorrectHeading);

    UNITY_END();
}

