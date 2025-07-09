#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X test");

  // Initialize I2C
  Wire.begin(25, 26);  // Wire.begin(sda, scl)

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }

  Serial.println("VL53L0X ready!");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data

  if (measure.RangeStatus != 4) {  // 4 means out of range
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }

  delay(100);
}