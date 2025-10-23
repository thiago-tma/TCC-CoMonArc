/*
 * Lista de Testes do UserInterface
 * Objetivo: Gerenciar controle de Leds, Campainhas e Botões
 * 
 * Módulo inicializa módulo GPIO
 * Módulo inicializa GPIOs usados durante inicialização                                     OK
 *      LED, Campainha e Botão                              OK
 *      Inicializa LED e Campainha como saídas e desligados OK
 *      Inicializa botão como entrada e pull-down           OK
 * Blink do LED com 1 repetição e 1 segundo
 *      InterfaceRun altera pino do LED
 *      Estado do pino se mantém com novo InterfaceRun se 1 segundo não passou
 *      Pino é desligado após 1 segundo
 * Chamar função de blink para pino com operação em andamento inicia nova operação imediatamente
 *      Blink do LED com 1 repetição e 1 segundo ON, passa 500ms e é chamada novamente
 * Blink do buzzer com 3 repetições, 500ms ligado e 100ms desligado
 *      Operação é feita no pino correto
 *      Verificação do pino em intervalos de 50 ms
 * Debounce do estado do botão (botão deve manter estado fixo por 50 ms para ser válido)
 * Transição do botão de baixo para alto (pino de baixo para alto) ativa função atrelada
 * 
*/

#include "unity.h"
#include "unity_fixture.h"
#include "UserInterface.h"
#include "FakeGPIO.h"

TEST_GROUP(UserInterface);

TEST_SETUP(UserInterface)
{
    UserInterface_Create();
}

TEST_TEAR_DOWN(UserInterface)
{
    UserInterface_Destroy();
}

TEST(UserInterface, CreateAndDestroy)
{
    UserInterface_Create();
    UserInterface_Destroy();
}

TEST(UserInterface, InitializeGPIOModuleOnInitialization)
{
    TEST_ASSERT_TRUE(FakeGPIO_GetInitialized());
}

TEST(UserInterface, SetPinsDirectionAndValueOnInitialization)
{
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_OUTPUT, FakeGPIO_GetPinDirection(BSP_PIN_LED), "LED Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_LED), "LED Value");
    
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_OUTPUT, FakeGPIO_GetPinDirection(BSP_PIN_BUZZER), "Buzzer Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_BUZZER), "Buzzer Value");

    TEST_ASSERT_EQUAL_MESSAGE(GPIO_DIR_INPUT, FakeGPIO_GetPinDirection(BSP_PIN_BUTTON),  "Button Direction");
    TEST_ASSERT_EQUAL_MESSAGE(GPIO_VALUE_LOW, FakeGPIO_GetPinValue(BSP_PIN_BUTTON), "Button Value");
}