/*
 * Lista de Testes do SoftTimer
 * Objetivo: Gerenciar serviço de alarmes/temporizadores
 * 
 * Cria temporizador com tempo limite                                                           OK
 *      período = 0 retorna falso           OK
 *      período != 0 retorna verdadeiro     OK
 * Check retorna false caso tempo limite não tenha sido atingido ainda                          OK
 * Check retorna true caso tempo limite tenha sido atingido e reinicia temporizador             OK
 * Check retorna false depois de retornar true e outro período ainda não passou                 OK
 * Check retorna true multiplas vezes sempre após o período de tempo do temporizador            OK
 * Se comporta corretamente durante overflow do tempo de sistema                                OK
 *      Retorna false próximo do overflow de tempo do sistema corretamente    OK    
 *      Retorna true próximo do overflow de tempo do sistema corretamente     OK    
 *      Retorna false corretamente mesmo após overflow de tempo               OK    
 *      Retorna true corretamente mesmo após overflow de tempo                OK     
 * Reiniciar temporizador zera tempo corrido (mesmo após período passar)                        OK     
 * Função para deletar temporizador
*/


#include "unity.h"
#include "unity_fixture.h"
#include "SystemClock.h"
#include "SoftTimer.h"
#include "FakeSystemTimer.h"

#define TIME_PERIOD 1000

TEST_GROUP(SoftTimer);

static SoftTimer testTimer;

TEST_SETUP(SoftTimer)
{
    testTimer.timerPeriod = 0;
    testTimer.startTime = 0;

    SystemClock_Create();
    SoftTimer_Create(&testTimer, TIME_PERIOD);
}

TEST_TEAR_DOWN(SoftTimer)
{
    SystemClock_Destroy();
}



TEST(SoftTimer, CreateAndDestroySoftwareTimer)
{
  SoftTimer_Create(&testTimer, TIME_PERIOD);
  SoftTimer_Destroy(&testTimer);
}

TEST(SoftTimer, TimerPeriodMustNotBeZeroInInitialization)
{
    TEST_ASSERT(SoftTimer_Create(&testTimer, TIME_PERIOD));
    TEST_ASSERT_FALSE(SoftTimer_Create(&testTimer, 0));
}

TEST(SoftTimer, CheckTimerReturnsFalseWhenItsNotTime)
{
  FakeSystemTimer_AddTime(TIME_PERIOD - 1);
  bool itsTime = SoftTimer_Check(&testTimer);
  
  TEST_ASSERT_FALSE(itsTime);
}

TEST(SoftTimer, CheckTimerReturnsTrueWhenTimerExpires)
{
  FakeSystemTimer_AddTime(TIME_PERIOD + 1);
  bool itsTime = SoftTimer_Check(&testTimer);

  TEST_ASSERT(itsTime);
}

TEST(SoftTimer, CheckTimerReturnsFalseJustAfterReturningTrue)
{
  bool itsTimeAgain;

  FakeSystemTimer_AddTime(TIME_PERIOD + 1);
  SoftTimer_Check(&testTimer);
  itsTimeAgain = SoftTimer_Check(&testTimer);

  TEST_ASSERT_FALSE(itsTimeAgain);
}

TEST(SoftTimer, CheckTimerReturnsTrueAgainAfterTimePeriod)
{
  FakeSystemTimer_AddTime(TIME_PERIOD + 1);
  TEST_ASSERT(SoftTimer_Check(&testTimer));
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(TIME_PERIOD/2);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(TIME_PERIOD);
  TEST_ASSERT(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(TIME_PERIOD);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(1);
  TEST_ASSERT(SoftTimer_Check(&testTimer));
}

TEST(SoftTimer, CheckTimeOnSystemTimeOverflow)
{
  /* Supõe-se que timeMicrosseconds seja unsigned */
  timeMicroseconds maxValue = -1;

  FakeSystemTimer_AddTime(maxValue);
  TEST_ASSERT(SoftTimer_Check(&testTimer)); 
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer)); 

  FakeSystemTimer_AddTime(TIME_PERIOD/2);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer)); 

  FakeSystemTimer_AddTime(TIME_PERIOD);
  TEST_ASSERT(SoftTimer_Check(&testTimer)); 
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer)); 
}

TEST(SoftTimer, ResetingTimerStartsOverCounting)
{
  FakeSystemTimer_AddTime(TIME_PERIOD+TIME_PERIOD/2);
  SoftTimer_Reset(&testTimer);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(10 + TIME_PERIOD/2);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));

  FakeSystemTimer_AddTime(10 + TIME_PERIOD/2);
  TEST_ASSERT(SoftTimer_Check(&testTimer));
}

TEST(SoftTimer, DestroyTimer)
{
  /* Supõe-se que timeMicrosseconds seja unsigned */
  timeMicroseconds maxValue = -1;

  SoftTimer_Destroy(&testTimer);
  FakeSystemTimer_AddTime(maxValue);
  TEST_ASSERT_FALSE(SoftTimer_Check(&testTimer));
  
}