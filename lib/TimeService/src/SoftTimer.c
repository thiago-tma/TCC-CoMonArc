#include <SoftTimer.h>
#include <SystemClock.h>

bool SoftTimer_Create (SoftTimer * softTimer, timeMicroseconds timerPeriod)
{
    if (!timerPeriod) return false;

    softTimer->startTime = SystemClock_Time();
    softTimer->timerPeriod = timerPeriod;
    softTimer->active = true;

    return true;
}

bool SoftTimer_Destroy (SoftTimer * softTimer)
{
    softTimer->active = false;
    return true;
}

bool SoftTimer_Check (SoftTimer * softTimer)
{
    if (softTimer->active == false) return false;

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