#include <SystemConfig.h>
#include <Arduino.h>
#include <WiFi.h>
#include <esp32-hal.h>

void SystemConfig_Init(void)
{
    setCpuFrequencyMhz(240);

    WiFi.mode(WIFI_OFF);
    btStop();

    disableCore0WDT();
}