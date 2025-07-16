#include <Wire.h>
#include <Adafruit_LIS3MDL.h>

Adafruit_LIS3MDL lis3mdl;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("LIS3MDL test");

  Wire.begin(25, 26);  // defaults to GPIO21 (SDA), GPIO22 (SCL) on ESP32

  if (!lis3mdl.begin_I2C()) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) delay(10);
  }

  Serial.println("LIS3MDL Found!");
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
}

void loop() {
  sensors_event_t event;
  lis3mdl.getEvent(&event);

  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print(" uT, ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print(" uT, ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.println(" uT");

  delay(100);
}
