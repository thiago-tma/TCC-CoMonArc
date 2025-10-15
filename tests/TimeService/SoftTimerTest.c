/*
 * Lista de Testes do SoftTimer
 * Objetivo: Gerenciar serviço de alarmes/temporizadores
 * 
 * Cria temporizador com tempo limite
 *      período = 0 retorna falso
 *      período != 0 retorna verdadeiro    
 * Check retorna false caso tempo limite não tenha sido atingido ainda               
 * Check retorna true caso tempo limite tenha sido atingido e reinicia temporizador  
 * Check retorna false depois de retornar true e outro período ainda não passou      
 * Check retorna true multiplas vezes sempre após o período de tempo do temporizador 
 * Se comporta corretamente durante overflow do tempo de sistema
 *      Retorna false próximo do overflow de tempo do sistema corretamente          
 *      Retorna true próximo do overflow de tempo do sistema corretamente           
 *      Retorna false corretamente mesmo após overflow de tempo                     
 *      Retorna true corretamente mesmo após overflow de tempo                      
 * Reiniciar temporizador zera tempo corrido (mesmo após período passar)           
 * Função para deletar temporizador
*/


#include "unity.h"
#include "unity_fixture.h"
#include "SystemClock.h"
#include "SoftTimer.h"

TEST_GROUP(SoftTimer);

static SoftTimer testTimer;

TEST_SETUP(SoftTimer)
{
    testTimer.timerPeriod = 0;
    testTimer.startTime = 0;
}

TEST_TEAR_DOWN(SoftTimer)
{

}



TEST(SoftTimer, CreateAndDestroySoftwareTimer)
{

  SoftTimer_Create(&testTimer, 1000);
  SoftTimer_Destroy(&testTimer);
}


/*
TEST(SoftTimer, CheckTimerReturnsFalseWhenItsNotTime)
{
  FakeSystemTimer_AddTime(5000);
  bool itsTime = TimeService_CheckTimer(testTimer);
  
  TEST_ASSERT(!itsTime);
}
*/