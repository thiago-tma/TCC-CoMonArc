#include <Arduino.h>
#include <Magnetometer.h>

void setup (void)
{
    Serial.begin(9600);
    Serial.println("Initializing magnetometer...");
    Magnetometer_Create();
    Serial.println("Magnetometer OK!");
}

void loop (void)
{
    int16_t headingRead = 0;

    unsigned long t0 = micros();
    Magnetometer_NewRead();
    unsigned long t1 = micros();

    Magnetometer_GetHeading(&headingRead);
    Serial.print("Magnetometer read: ");
    Serial.println(headingRead);

    unsigned long elapsed = t1 - t0;
    Serial.print("Time to take and process new read (us): ");
    Serial.println(elapsed);
    delay(1000);
}