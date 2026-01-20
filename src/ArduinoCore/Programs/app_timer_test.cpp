#include <Arduino.h>
#include <SystemClock.h>
#include <SoftTimer.h>

static SoftTimer myTimer;

void setup (void)
{
    Serial.begin(9600);
    SystemClock_Create();
    SoftTimer_Create(&myTimer, 1000000UL);
    pinMode(LED_BUILTIN, OUTPUT); /*Pino 13 no arduino*/
}

void loop (void)
{
    static unsigned long t1 = 0, timePassed;
    if(SoftTimer_Check(&myTimer))
    {
        timePassed = (millis()-t1);
        t1 = millis();
        Serial.print("Time passed: ");
        Serial.println(timePassed);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}