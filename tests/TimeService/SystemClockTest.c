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
 * Retorna o número do slot ocupado ao registrar função de callback
 * Executa a função de callback registrada durante tique do sistema
 * Registra múltiplas funções de callback e as executa durante tique do sistema
 * Destruir módulo remove funções de callback registradas
 * Retorna -1 caso todos os slots de callback estejam ocupados
 * Remove função de callback mediante o número do slot passado
 *      
*/


#include "SystemClock.h"
#include "FakeSystemTimer.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(SystemClock);

static timeMicroseconds time;

TEST_SETUP(SystemClock)
{
  SystemClock_Create();
  time = -1;
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
