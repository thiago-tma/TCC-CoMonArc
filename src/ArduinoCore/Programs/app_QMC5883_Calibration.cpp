#include <Wire.h>
#include <Adafruit_QMC5883P.h>

Adafruit_QMC5883P qmc;

// Initialize min/max with extreme values
int16_t xMin = 32767, xMax = -32768;
int16_t yMin = 32767, yMax = -32768;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!qmc.begin()) {
    Serial.println("QMC5883P not found");
    while (1);
  }

  // Recommended configuration for calibration
  qmc.setRange(QMC5883P_RANGE_2G);
  qmc.setOSR(QMC5883P_OSR_4);
  qmc.setDSR(QMC5883P_DSR_2);
  qmc.setODR(QMC5883P_ODR_50HZ);
  qmc.setMode(QMC5883P_MODE_CONTINUOUS);

  Serial.println("Hard-iron calibration");
  Serial.println("Rotate sensor slowly in all directions...");
}

void loop() {
  int16_t x, y, z;

  // Read raw magnetic field data
  qmc.getRawMagnetic(&x, &y, &z);

  // Update min/max
  if (x < xMin) xMin = x;
  if (x > xMax) xMax = x;
  if (y < yMin) yMin = y;
  if (y > yMax) yMax = y;

  // Print live values
  Serial.print("Xmin: "); Serial.print(xMin);
  Serial.print("Xmax: "); Serial.print(xMax);
  Serial.print("Ymin: "); Serial.print(yMin);
  Serial.print("Ymax: "); Serial.println(yMax);

  delay(100);
}