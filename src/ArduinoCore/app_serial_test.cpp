#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    Serial.print("RX: ");
    Serial.println(line);
  }
}