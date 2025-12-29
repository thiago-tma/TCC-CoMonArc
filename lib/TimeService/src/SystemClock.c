#include <stdlib.h>

#include <SystemClock.h>
#include <SystemTimer.h>

typedef struct callBackSlot
{
    callBack function;
    bool occupied;
} callBackSlot;


// Esta variável guarda o tempo de sistema
// Como é atualizada por meio de interrupção, deve ser do tipo volátil (volatile)
static volatile timeMicroseconds systemTime = 0;

static volatile callBackSlot storedCallBacks[MAX_REGISTERED_CALLBACKS];

static int getAvailableCallBackSlot(void)
{
    for (int i = 0; i < MAX_REGISTERED_CALLBACKS; i++)
    {
        if (!storedCallBacks[i].occupied) return i;
    }

    return -1;
}

static void runCallBacks(void)
{
    for (int i = 0; i < MAX_REGISTERED_CALLBACKS; i++)
    {
        if (storedCallBacks[i].occupied) storedCallBacks[i].function();
    }
}

static void clearStoredCallBacks (void)
{
    for (int i = 0; i < MAX_REGISTERED_CALLBACKS; i++)
    {
        storedCallBacks[i].occupied = false;
    }
}

static void timerInterruptCallback(timeMicroseconds useconds)
{
    systemTime += useconds;

    runCallBacks();
}

void SystemClock_Create(void)
{
    clearStoredCallBacks();

    SystemTimer_Create(timerInterruptCallback);
}

void SystemClock_Destroy(void)
{
    SystemTimer_Destroy();
    systemTime = 0;

    clearStoredCallBacks();
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

int SystemClock_AddCallback (callBack function)
{
    if (!function) return -2;

    int availableSlot = getAvailableCallBackSlot();

    if (availableSlot == -1) return -1;
    
    storedCallBacks[availableSlot].function = function;
    storedCallBacks[availableSlot].occupied = true;

    return availableSlot;
}

bool SystemClock_RemoveCallback (int functionId)
{
    if ((functionId < 0) || (functionId >= MAX_REGISTERED_CALLBACKS)) return false;

    if (storedCallBacks[functionId].occupied == true)
    {
        storedCallBacks[functionId].occupied = false;
        return true;
    }

    return false;
}