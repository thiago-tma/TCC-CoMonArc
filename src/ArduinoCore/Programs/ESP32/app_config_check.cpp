#include <Arduino.h>
#include <esp32-hal.h>
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup() {
  setCpuFrequencyMhz(240);

  Serial.begin(115200);
  delay(1000);

  Serial.println("===== ESP32 CONFIG VERIFICATION =====");

  // CPU frequency
  Serial.print("CPU Frequency (MHz): ");
  Serial.println(getCpuFrequencyMhz());

  // Core info
  Serial.print("portNUM_PROCESSORS: ");
  Serial.println(portNUM_PROCESSORS);

  Serial.print("Running on core: ");
  Serial.println(xPortGetCoreID());

#ifdef CONFIG_FREERTOS_UNICORE
  Serial.println("CONFIG_FREERTOS_UNICORE: ENABLED");
#else
  Serial.println("CONFIG_FREERTOS_UNICORE: DISABLED");
#endif

  // FreeRTOS tick rate
  Serial.print("FreeRTOS Tick Rate (Hz): ");
  Serial.println(configTICK_RATE_HZ);

  // Bluetooth
#ifdef CONFIG_BT_ENABLED
  Serial.println("Bluetooth: ENABLED");
#else
  Serial.println("Bluetooth: DISABLED");
#endif

  // Watchdog
#ifdef CONFIG_ESP_TASK_WDT
  Serial.println("Task Watchdog: ENABLED");
#else
  Serial.println("Task Watchdog: DISABLED");
#endif

  // Log level
  Serial.print("ESP_LOG_DEFAULT_LEVEL: ");
#ifdef CONFIG_LOG_DEFAULT_LEVEL
  Serial.println(CONFIG_LOG_DEFAULT_LEVEL);
#else
  Serial.println("Not defined");
#endif

  // Heap info
  Serial.print("Free heap (bytes): ");
  Serial.println(ESP.getFreeHeap());

  // USB CDC
#ifdef ARDUINO_USB_CDC_ON_BOOT
  Serial.println("USB CDC on boot: ENABLED");
#else
  Serial.println("USB CDC on boot: DISABLED");
#endif

  Serial.println("=====================================");
}

void loop() {
  static uint32_t counter = 0;
  Serial.print("Loop running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" | counter = ");
  Serial.println(counter++);
  delay(1000);
}