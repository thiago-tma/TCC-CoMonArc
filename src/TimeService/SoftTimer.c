#include "SoftTimer.h"
#include "SystemClock.h"

bool SoftTimer_Create (SoftTimer * softTimer, timeMicroseconds timerPeriod)
{
    if (!timerPeriod) return false;

    softTimer->startTime = SystemClock_Time();
    softTimer->timerPeriod = timerPeriod;

    return true;
}

bool SoftTimer_Destroy (SoftTimer * softTimer)
{
    /* Maximiza periodo para forçar o retorno de falso ao checar timer  */
    softTimer->timerPeriod = -1;
    return true;
}

bool SoftTimer_Check (SoftTimer * softTimer)
{
    timeMicroseconds currentTime = SystemClock_Time();

    /* Esta equação funciona mesmo para casos de overflow, mas depende das variáveis serem 'unsigned' */
    if (  softTimer->timerPeriod < (currentTime - softTimer->startTime))
    {
        softTimer->startTime = currentTime;
        return true;
    }

    return false;
}

void SoftTimer_Reset (SoftTimer * softTimer)
{
    softTimer->startTime = SystemClock_Time();
}