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
 * Primeiro Run() (antes de timeout) posiciona servo em 90 deg                              OK
 * Run() toma sensor como referência após timeout                                           OK
 *      Run() consequente corrige direção                                                   OK
 * Reset() realinha direção em 90º                                                          OK
 *      Run() pós timeout toma nova leitura de sensor como referência                       OK
 *      Run() em sequência corrige direção                                                  OK
 * Controle de servo é limitado entre 0 a 180 mesmo passando do limite                      OK
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

static void nextRunAfterTimeout(int16_t magnetometerReading)
{
    FakeSystemTimer_AddTime(YAWCONTROLLER_RESET_TIME_MICROSECONDS+1);
    FakeMagnetometer_SetReading(magnetometerReading);
    YawController_Run();
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

void test_FirstRunSetsServoOn90Deg (void)
{
    YawController_Run();
    TEST_ASSERT_EQUAL(90, FakeServoController_ReadHeading());
}

void test_GetReferenceAfterTimeoutAndCorrectHeading (void)
{
    FakeMagnetometer_SetReading(45);
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "Timer has not expired yet");

    nextRunAfterTimeout(60);
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "YawController must only get the reference signal");

    nextRunAfterTimeout(60-5);
    TEST_ASSERT_EQUAL_MESSAGE(90+5, FakeServoController_ReadHeading(), "First yaw correction failed");

    nextRunAfterTimeout(60+5);
    TEST_ASSERT_EQUAL_MESSAGE(90+5-5, FakeServoController_ReadHeading(),  "Second yaw correction failed");
}

void test_ResetAction (void)
{
    nextRunAfterTimeout(60);
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "YawController must only get the reference signal");
    nextRunAfterTimeout(60-5);
    TEST_ASSERT_EQUAL_MESSAGE(90+5, FakeServoController_ReadHeading(), "First yaw correction failed");
    YawController_Reset();
    TEST_ASSERT_EQUAL_MESSAGE(90+5, FakeServoController_ReadHeading(), "YawController waits for next run to change servo back to 90 deg");
    YawController_Run();
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "YawController resets servo to 90 deg");
    nextRunAfterTimeout(120);
    TEST_ASSERT_EQUAL_MESSAGE(90, FakeServoController_ReadHeading(), "YawController only gets the new reference (120 deg)");
    nextRunAfterTimeout(120+30);
    TEST_ASSERT_EQUAL_MESSAGE(90-30, FakeServoController_ReadHeading(), "Yaw correction after reset failed");
}

void test_YawControllerLimitsServoBetween0And180Deg (void)
{
    nextRunAfterTimeout(60); /*Reference set at 60 deg */
    nextRunAfterTimeout(60+90); /*Magnetometer says heading is 90 deg beyond the reference*/
    TEST_ASSERT_EQUAL_MESSAGE(90-90, FakeServoController_ReadHeading(), "YawCorrection failed (1)"); /*Servo set to 0 deg*/
    nextRunAfterTimeout(60+5); /*Magnetometer says heading is 5 deg beyond the reference*/
    TEST_ASSERT_EQUAL_MESSAGE(90-90/*-5*/, FakeServoController_ReadHeading(), "YawCorrection failed (2)"); /*Servo set to 0 deg, can't go further*/
    nextRunAfterTimeout(60-180); /*Magnetometer says heading is 180 deg behind the reference*/
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180, FakeServoController_ReadHeading(), "YawCorrection failed (3)"); /*Servo set to 180 deg*/
    nextRunAfterTimeout(60-5); /*Magnetometer says heading is 5 deg behind the reference*/
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180/*+5*/, FakeServoController_ReadHeading(), "YawCorrection failed (4)"); /*Servo set to 180 deg, servo can't go further*/
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
    RUN_TEST(test_FirstRunSetsServoOn90Deg);
    RUN_TEST(test_GetReferenceAfterTimeoutAndCorrectHeading);
    RUN_TEST(test_ResetAction);
    RUN_TEST(test_YawControllerLimitsServoBetween0And180Deg);

    UNITY_END();
}

