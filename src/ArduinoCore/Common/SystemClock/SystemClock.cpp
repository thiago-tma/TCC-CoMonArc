#include <Arduino.h>
#include <SystemClock.h>

void SystemClock_Create (void){}

void                SystemClock_Destroy     (void){}

timeMicroseconds    SystemClock_Time        (void)
{
    return micros();
}

int SystemClock_AddCallback (callBack function)
{
    return -1;
}

bool                 SystemClock_RemoveCallback (int functionId)
{
    return false;
}
