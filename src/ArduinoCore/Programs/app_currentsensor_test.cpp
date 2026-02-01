#include <Arduino.h>
#include <CurrentSensor.h>

void setup (void)
{
    Serial.begin(115200);
    Serial.println("Initializing Current sensor...");
    CurrentSensor_Create();
    Serial.println("Current sensor OK!");
}

void loop (void)
{
    Current_Microamps_t currentRead = 0;

    CurrentSensor_NewRead();
    CurrentSensor_GetCurrent(&currentRead);

    Serial.print("Corrente: ");
    Serial.print(currentRead);
    Serial.println(" uA");

    delay(1000);
}