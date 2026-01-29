/*
 *  Lista de testes do YawController
 *  Objetivo: Controlar o servo motor para manter a guinada indicada pelo Magnetometro constante
 *  Descriçâo: Inicia (e reinicia) com haste em 90º. Após um intervalo definido, toma a leitura atual do sensor como referência
 *      e conserva a direção dentro dos limites de atuação da haste.
 * 
 * TESES SUPOEM 
 * YAWCONTROLLER_PROPORTIONAL_MULTIPLER 1
 * YAWCONTROLLER_PROPORTIONAL_DIVIDER   1
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
 * Controlle se comporta bem com referência próxima de 0 (passando para 360)                OK
 * Controlle se comporta bem com referência próxima de 360 (passando para 0)                OK
 * Controle de servo vai para outro extremo quando referência passa por '270º'              OK
 *      Histerese entre trocas de extremos(?)
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

void test_ResetRedefinesReferenceAfterTimeout (void)
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

void test_YawControllerWithReferenceCloseTo0Deg (void)
{
    nextRunAfterTimeout(10); /*Reference set at 10 deg */
    nextRunAfterTimeout(350); /*Magnetometer says heading is 20 deg behind the reference, crossing zero */
    TEST_ASSERT_EQUAL_MESSAGE(90+20, FakeServoController_ReadHeading(), "YawCorrection failed (1)"); /*Servo set to 110 deg*/
}

void test_YawControllerWithReferenceCloseTo360Deg (void)
{
    nextRunAfterTimeout(350); /*Reference set at 10 deg */
    nextRunAfterTimeout(50); /*Magnetometer says heading is 20 deg behind the reference, crossing zero */
    TEST_ASSERT_EQUAL_MESSAGE(90-60, FakeServoController_ReadHeading(), "YawCorrection failed (1)"); /*Servo set to 30 deg*/
}

void test_YawControllerSwitchesExtremesWhenMagnetometerWrapsAround (void)
{
    nextRunAfterTimeout(90); /*Reference set at 90 deg */
    nextRunAfterTimeout(90+90); /* Magnetometer says heading is 90 deg beyond reference */
    TEST_ASSERT_EQUAL_MESSAGE(90-90, FakeServoController_ReadHeading(), "YawCorrection failed (1)"); /*Servo set to 0 deg*/
    nextRunAfterTimeout(90+90); /* Magnetometer says heading is 90 deg beyond reference */
    TEST_ASSERT_EQUAL_MESSAGE(90-90/*-90*/, FakeServoController_ReadHeading(), "YawCorrection failed (2)"); /*Servo still set to 0 deg*/
    nextRunAfterTimeout(90+90+1);
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180, FakeServoController_ReadHeading(), "YawCorrection failed (3)"); /*Servo switches to the other extreme*/
    nextRunAfterTimeout(90-89); /* Magnetometer now says heading is 89 deg behind reference */
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180, FakeServoController_ReadHeading(), "YawCorrection failed (4)"); /*Servo stays other extreme*/
    nextRunAfterTimeout(90-90);
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180, FakeServoController_ReadHeading(), "YawCorrection failed (5)"); /*Servo stays other extreme*/
    nextRunAfterTimeout(90-91+360);
    TEST_ASSERT_EQUAL_MESSAGE(90-90+180-180, FakeServoController_ReadHeading(), "YawCorrection failed (6)"); /*Servo switches back to the other extreme*/
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
    RUN_TEST(test_ResetRedefinesReferenceAfterTimeout);
    RUN_TEST(test_YawControllerLimitsServoBetween0And180Deg);
    RUN_TEST(test_YawControllerWithReferenceCloseTo0Deg);
    RUN_TEST(test_YawControllerWithReferenceCloseTo360Deg);
    RUN_TEST(test_YawControllerSwitchesExtremesWhenMagnetometerWrapsAround);

    UNITY_END();
}

