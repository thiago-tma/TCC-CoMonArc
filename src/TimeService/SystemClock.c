#include <stdlib.h>

#include "SystemClock.h"
#include "SystemTimer.h"

// Esta variável guarda o tempo de sistema
// Como é atualizada por meio de interrupção, deve ser do tipo volátil (volatile)
static volatile timeMicroseconds systemTime = 0;

static void timerInterruptCallback(timeMicroseconds useconds)
{
    systemTime += useconds;
}

void SystemClock_Create(void)
{
    SystemTimer_Create(timerInterruptCallback);
}

void SystemClock_Destroy(void)
{
    SystemTimer_Destroy();
    systemTime = 0;
}

static void readCurrentTime(timeMicroseconds * time)
{
    SystemTimer_Pause();
    *time = systemTime;
    SystemTimer_Continue();
}

timeMicroseconds SystemClock_Time(void)
{
    timeMicroseconds time = 0;
    readCurrentTime(&time);
    return time;
}

