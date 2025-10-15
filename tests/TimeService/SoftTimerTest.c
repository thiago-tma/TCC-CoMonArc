/*
 * Lista de Testes do SoftTimer
 * Objetivo: Gerenciar serviço de alarmes/temporizadores
 * 
 * Cria temporizador com tempo limite (período para retornar verdadeiro)     
 * Retorna false caso tempo limite não tenha sido atingido ainda               
 * Retorna true caso tempo limite tenha sido atingido e reinicia temporizador  
 * Retorna false depois de retornar true e outro período ainda não passou      
 * Retorna true multiplas vezes sempre após o período de tempo do temporizador 
 * Retorna false próximo do overflow de tempo do sistema corretamente          
 * Retorna true próximo do overflow de tempo do sistema corretamente           
 * Retorna false corretamente mesmo após overflow de tempo                     
 * Retorna true corretamente mesmo após overflow de tempo                      
 * Função para reiniciar temporizador                                          
 * Função para deletar temporizador                                            
 * O que fazer se for passado tempo limite 0?
 *      
*/


#include "unity.h"
#include "unity_fixture.h"
#include "SystemClock.h"
#include "SoftTimer.h"

TEST_GROUP(SoftTimer);

static SoftTimer testTimer;

TEST_SETUP(SoftTimer)
{
    /*
    SystemClock_Create();
    testTimer = TimeService_CreateTimer(10000);
    */
}

TEST_TEAR_DOWN(SoftTimer)
{
    /*
    TimeService_DestroyTimer(testTimer);
    TimeService_Destroy();
    */
}

/*

TEST(SoftTimer, CreateAndDestroySoftwareTimer)
{
  // Para implementação que utilize alocação dinâmica de memória
  // Este teste depende da configuração do Unity de verificação de vazamento de memória estar ligada
  SoftTimer myTimer = TimeService_CreateTimer(10000);
  TimeService_DestroyTimer(myTimer);
}

TEST(SoftTimer, CheckTimerReturnsFalseWhenItsNotTime)
{
  FakeSystemTimer_AddTime(5000);
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(!itsTime);
}

TEST(SoftTimer, CheckTimerReturnsTrueWhenItsTime)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime);
}

TEST(SoftTimer, CheckTimerReturnsFalseAfterReturningTrueAndItsNotTimeYetAgain)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
 
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(SoftTimer, CheckTimerReturnsTrueAfterAnotherPeriod)
{
  FakeSystemTimer_AddTime(15000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  FakeSystemTimer_AddTime(10000);
  bool itsTime3 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
  TEST_ASSERT(itsTime3);
}

TEST(SoftTimer, CheckTimerReturnsFalseBeforeTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService próximo do overflow,
  // com o próximo 'alarme' marcado para depois do overflow e verifica que o teste retona false
  // antes do overflow e antes deste período.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX-500);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(10);
  bool itsTime2 = TimeService_CheckTimer(testTimer);

  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(SoftTimer, CheckTimerReturnsTrueBeforeTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService próximo do overflow,
  // com o próximo 'alarme' marcado para antes do overflow e verifica que o teste retona true
  // antes do overflow e depois deste período.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX-20000);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(15000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(itsTime2);
}

TEST(SoftTimer, CheckTimerReturnsFalseAfterTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService depois do overflow,
  // e verifica que a função retorna falso corretamente.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(10);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(!itsTime2);
}

TEST(SoftTimer, CheckTimerReturnsTrueAfterTimeOverflows)
{
  // Este teste põe o tempo de sistema guardado por TimeService depois do overflow,
  // e verifica que a função retorna verdadeiro corretamente.
  // Este teste supõe que o tempo de sistema está guardada numa variável tipo unsigned long
  FakeSystemTimer_AddTime(ULONG_MAX);
  bool itsTime1 = TimeService_CheckTimer(testTimer);
  
  FakeSystemTimer_AddTime(20000);
  bool itsTime2 = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(itsTime1);
  TEST_ASSERT(itsTime2);
}

TEST(SoftTimer, CheckTimerReturnsFalseAfterResetingTimer)
{
  FakeSystemTimer_AddTime(9000);
  TimeService_ResetTimer(testTimer);
  FakeSystemTimer_AddTime(5000);
  
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(!itsTime);
}

*/