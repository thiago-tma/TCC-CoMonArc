/*
 * Lista de Testes do SystemClock
 * Objetivo: Manter o tempo de sistema e gerenciar serviço de alarmes/temporizadores
 * 
 * Tempo de sistema é zero ao criar o módulo                                            OK
 * Tempo do sistema aumenta quando função de callback é chamada por X microsegundos     OK
 * Módulo pode ser inicializado N vezes sem alterar relógio do sistema                  OK
 * Tempo de sistema é zero após destruir e criar módulo novamente                       OK
 * Leitura do tempo não é corrompida por interrupção (feito por pausa e continuação)    OK?
 * 
 * Retorna o número do slot ocupado ao registrar função de callback                     OK
 * Destruir módulo remove funções de callback registradas                               OK
 * Executa a função de callback registrada durante tique do sistema                     OK
 * Registra múltiplas funções de callback e as executa durante tique do sistema         OK
 * Retorna -1 caso todos os slots de callback estejam ocupados                          OK
 * Remove função de callback mediante o número do slot passado
 *      
*/


#include "SystemClock.h"
#include "FakeSystemTimer.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(SystemClock);

static timeMicroseconds time;
static int callBackVar = 0;

static void testCallBack1(void)
{
    callBackVar += 1;
}

static void testCallBack2(void)
{
    callBackVar += 10;
}

TEST_SETUP(SystemClock)
{
  SystemClock_Create();
  time = -1;

  callBackVar = 0;
}

TEST_TEAR_DOWN(SystemClock)
{
  SystemClock_Destroy();
}

TEST(SystemClock, TimeIsZeroAfterCreate)
{
  time = SystemClock_Time();
  TEST_ASSERT(time == 0);
}

TEST(SystemClock, TimeIncreasedAfterSystemCallback)
{
  FakeSystemTimer_AddTime(1000);
    
  time = SystemClock_Time();

  TEST_ASSERT_EQUAL_INT(1000, time);
}

TEST(SystemClock, MultipleInitializatonsDoNotChangeTime)
{
  FakeSystemTimer_AddTime(1000);
  SystemClock_Create();
  time = SystemClock_Time();

  TEST_ASSERT_EQUAL_INT(1000, time);
}

TEST(SystemClock, TimeResetAfterDestroy)
{
  FakeSystemTimer_AddTime(1000);
  SystemClock_Destroy();
  SystemClock_Create();
  
  time = SystemClock_Time();

  TEST_ASSERT_EQUAL_INT(0, time);
}

TEST(SystemClock, TimeReadIsInterruptProof)
{
  /*  Este teste não garante que a leitura do tempo não será interrompida
      Só verifica se houve desabilitação e habilitação da interrupção em algum momento ao pedir uma leitura
  */
  time = SystemClock_Time();
  time = SystemClock_Time();
  time = SystemClock_Time();

  int pauses = FakeSystemTimer_GetPauses();
  int continues = FakeSystemTimer_GetContinues();

  TEST_ASSERT_EQUAL_INT(3, pauses);
  TEST_ASSERT_EQUAL_INT(3, continues);
}

TEST(SystemClock, ReturnTrueWhenSuccessfullyRegisteringCallBackFunction)
{
  int slotId = -1;
  slotId = SystemClock_AddCallback(testCallBack1);

  TEST_ASSERT_EQUAL(0, slotId); 
}

TEST(SystemClock, DestroyingModuleResetsRegisteredCallBacks)
{
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, SystemClock_AddCallback(testCallBack1), "Slot returned != 0");
  SystemClock_Destroy();
  FakeSystemTimer_AddTime(1); 

  TEST_ASSERT_EQUAL_INT32(0, callBackVar);
}

TEST(SystemClock, RunCallBackFunctionOnSystemTick)
{
  SystemClock_AddCallback(testCallBack1);
  FakeSystemTimer_AddTime(1);   /* Tempo adicionado não importa */

  TEST_ASSERT_EQUAL_INT32(1, callBackVar);
}

TEST(SystemClock, RunMultipleCallBackFunctionOnSystemTick)
{
  SystemClock_AddCallback(testCallBack1);
  SystemClock_AddCallback(testCallBack2);

  FakeSystemTimer_AddTime(1);   /* Tempo adicionado não importa */

  TEST_ASSERT_EQUAL_INT32(11, callBackVar);
}

TEST(SystemClock, AddCallBackReturnsMinus1IfNoSlotAvailable)
{
  for (int i = 0; i < MAX_REGISTERED_CALLBACKS; i++)
  {
    SystemClock_AddCallback(testCallBack1);
  }

  TEST_ASSERT_EQUAL_INT32(-1, SystemClock_AddCallback(testCallBack1));
}

TEST(SystemClock, RemoveCallBack)
{
  for (int i = 0; i < MAX_REGISTERED_CALLBACKS; i++)
  {
    SystemClock_AddCallback(testCallBack2);
  }

  TEST_ASSERT_MESSAGE(SystemClock_RemoveCallback(0), "First slot not removed");
  TEST_ASSERT_FALSE_MESSAGE(SystemClock_RemoveCallback(0), "First slot already removed");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, SystemClock_AddCallback(testCallBack1), "First slot should have been available");

  FakeSystemTimer_AddTime(1); 

  TEST_ASSERT_EQUAL_INT32_MESSAGE((MAX_REGISTERED_CALLBACKS-1)*10 + 1, callBackVar, "Wrong value obtained from run callbacks");
}