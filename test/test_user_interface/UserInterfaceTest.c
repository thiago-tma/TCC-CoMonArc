/*
 * Lista de Testes do UserInterface
 * Objetivo: Gerenciar controle de Leds, Campainhas e Botões
 * 
 * Módulo inicializa módulo GPIO                                                                        OK
 * Módulo inicializa GPIOs usados durante inicialização                                                 OK
 *      LED, Campainha e Botão                                                                          ok
 *      Inicializa LED e Campainha como saídas e desligados                                             ok
 *      Inicializa botão como entrada e pull-down                                                       ok
 * Blink do LED com 1 repetição e 1 segundo                                                             OK
 *      InterfaceRun altera pino do LED                                                                 ok
 *      Estado do pino se mantém com novo InterfaceRun se 1 segundo não passou                          ok
 *      Pino é desligado após 1 segundo                                                                 ok
 * Blink com zero repetições desliga atuador imediatamente                                              OK
 * Chamar função de blink para pino com operação em andamento sobreescreve blink anterior               OK
 *      Blink do LED com 1 repetição e 1 segundo ON, passa 500ms e é chamada novamente                  ok
 * Blink do buzzer com 3 repetições, 500ms ligado e 100ms desligado                                     OK
 *      Operação é feita no pino correto                                                                ok
 *      Pino alterna estados após períodos selecionados                                                 ok    
 * Destroy/DeInit do módulo desliga atuadores
 *             
 * Debounce do estado do botão (botão deve manter estado fixo por 50 ms para ser válido)                OK
 *      Pressionamento do botão pós debounce (50ms ativo) ativa função atrelada                         ok
 *      Pressionamento do botão pós debounce sem função atrelada não tem efeito                         ok
 *      Pressionar o botão por 20ms, soltar e pressionar novamente ativa função somente aos 70ms        ok
 *      Para botão ir de pressionado para não pressionado, deve ficar não pressionado por 50ms          ok
 *      Ativar botão duas vezes                                                                         ok
 * 
*/

#include <unity.h>
#include <UserInterface.h>
#include "FakeGPIO.h"
#include <SystemClock.h>
#include "FakeSystemTimer.h"

static unsigned int timesCallbackCalled = 0;

static void testCallback(void)
{
    timesCallbackCalled++;
}


void setUp (void)
{
    timesCallbackCalled = 0;

    FakeGPIO_Reset();
    SystemClock_Create();

    UserInterface_Create();
}

void tearDown (void)
{
    UserInterface_Destroy();
    SystemClock_Destroy();
}

void test_CreateAndDestroy (void)
{
    UserInterface_Create();
    UserInterface_Destroy();
}

void test_InitializeGPIOModuleOnInitialization (void)
{
    TEST_ASSERT_TRUE(FakeGPIO_GetInitialized());
}

void test_SetPinsDirectionAndValueOnInitialization (void)
{
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_OUTPUT, FakeGPIO_GetPinDirection(BSP_PIN_LED), "LED Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_LED), "LED Value");
    
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_OUTPUT, FakeGPIO_GetPinDirection(BSP_PIN_BUZZER), "Buzzer Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer Value");

    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_INPUT, FakeGPIO_GetPinDirection(BSP_PIN_BUTTON),  "Button Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_BUTTON), "Button Value");
}

void test_BlinkLEDOneTime (void)
{
    UserInterface_BlinkComponent(ACTUATOR_LED, 1, 1000000, 0);

    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is off after Blink command");

    FakeSystemTimer_AddTime(500000);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is off too early");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is still on when it should not");
}

void test_CallBlinkLedWithZeroRepetitionsTurnsOffActuator (void)
{
    UserInterface_BlinkComponent(ACTUATOR_LED, 1, 1000000, 0);
    UserInterface_Run();

    FakeSystemTimer_AddTime(500000);
    UserInterface_Run();
    UserInterface_BlinkComponent(ACTUATOR_LED, 0, 1000000, 0);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED should be turned off");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED should still be off");
}

void test_CallBlinkLedWhileAlreadyBlinkingOverwritesLastBlinkCommand (void)
{
    UserInterface_BlinkComponent(ACTUATOR_LED, 1, 1000000, 0);
    UserInterface_Run();

    FakeSystemTimer_AddTime(500000);
    UserInterface_Run();

    UserInterface_BlinkComponent(ACTUATOR_LED, 1, 1000000, 0);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is off after Blink command");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is off too early");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_LED), "LED is still on when it should not");
}

void test_BlinkBuzzerThreeTimes500msOnAnd100msOff (void)
{
    UserInterface_BlinkComponent(ACTUATOR_BUZZER, 3, 500000, 100000);

    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is off after Blink command");

    FakeSystemTimer_AddTime(250000);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is off too early");

    FakeSystemTimer_AddTime(250001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is still on first blink");

    FakeSystemTimer_AddTime(100001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is still off second blink");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is still on second blink");

    FakeSystemTimer_AddTime(100001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is still off third blink");

    FakeSystemTimer_AddTime(500001);
    UserInterface_Run();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW ,FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer is still on third blink");
}

void test_DestroyTurnsOffAllActuators (void)
{
    UserInterface_BlinkComponent(ACTUATOR_BUZZER, 1, 500000, 0);
    UserInterface_BlinkComponent(ACTUATOR_LED, 1, 500000, 0);
    UserInterface_Run();

    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH, FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer should be active");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_HIGH, FakeGPIO_GetPinValue(BSP_PIN_LED), "Led should be active");
    
    UserInterface_Destroy();
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer should not be active");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_LED), "Led should not be active");
}

void test_DebouncedButtonActivatesCallback (void)
{
    Button_State_t buttonState;

    UserInterface_AddButtonFunction(testCallback);

    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_INACTIVE, buttonState, "Button should be inactive");
    TEST_ASSERT_EQUAL_MESSAGE(0, timesCallbackCalled, "Button activates function when it shouldn't");

    FakeSystemTimer_AddTime(50001);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_ACTIVE, buttonState, "Button should be active");
    TEST_ASSERT_EQUAL_MESSAGE(1, timesCallbackCalled, "Button should activate callback function");
}

void test_ButtonPressWithoutCallbackHasNoEffect (void)
{
    Button_State_t buttonState;

    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();
    FakeSystemTimer_AddTime(50001);
    UserInterface_Run();

    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_ACTIVE, buttonState, "Button should be active");
    TEST_ASSERT_EQUAL_MESSAGE(0, timesCallbackCalled, "Button should activate callback function");
}

void test_ButtonBounceIsIgnored (void)
{
    Button_State_t buttonState;

    UserInterface_AddButtonFunction(testCallback);
    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();

    FakeSystemTimer_AddTime(20000);
    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_LOW);
    UserInterface_Run();
    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();

    FakeSystemTimer_AddTime(30001);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_INACTIVE, buttonState, "Button should be inactive");
    TEST_ASSERT_EQUAL_MESSAGE(0, timesCallbackCalled, "Button should not have activated callback function");

    FakeSystemTimer_AddTime(20000);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_ACTIVE, buttonState, "Button should be active");
    TEST_ASSERT_EQUAL_MESSAGE(1, timesCallbackCalled, "Button should have activated callback function");
}

void test_PressButtonTwice (void)
{
    Button_State_t buttonState;
    UserInterface_AddButtonFunction(testCallback);

    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();
    
    FakeSystemTimer_AddTime(50001);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_ACTIVE, buttonState, "Button should be active");

    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_LOW);
    UserInterface_Run();
    FakeSystemTimer_AddTime(50001);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_INACTIVE, buttonState, "Button should be inactive");

    FakeGPIO_SetPinValue(BSP_PIN_BUTTON, GPIO_VALUE_HIGH);
    UserInterface_Run();
    FakeSystemTimer_AddTime(50001);
    UserInterface_Run();
    UserInterface_ReadButton(&buttonState);
    TEST_ASSERT_EQUAL_MESSAGE(BUTTON_ACTIVE, buttonState, "Button should be active");
    TEST_ASSERT_EQUAL_MESSAGE(2, timesCallbackCalled, "Callback should have been called twice");
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_CreateAndDestroy);
    RUN_TEST(test_InitializeGPIOModuleOnInitialization);
    RUN_TEST(test_SetPinsDirectionAndValueOnInitialization);
    RUN_TEST(test_BlinkLEDOneTime);
    RUN_TEST(test_CallBlinkLedWithZeroRepetitionsTurnsOffActuator);
    RUN_TEST(test_CallBlinkLedWhileAlreadyBlinkingOverwritesLastBlinkCommand);
    RUN_TEST(test_BlinkBuzzerThreeTimes500msOnAnd100msOff);
    RUN_TEST(test_DestroyTurnsOffAllActuators);
    RUN_TEST(test_DebouncedButtonActivatesCallback);
    RUN_TEST(test_ButtonPressWithoutCallbackHasNoEffect);
    RUN_TEST(test_ButtonBounceIsIgnored);
    RUN_TEST(test_PressButtonTwice);

    UNITY_END();
}