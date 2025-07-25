#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../lib/GlobalConstants.h"



void objectDetected(void *parameter) {
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  int lastmeasure = 0;
  int section = 0;
  int thresholds[2] = {DOORWAY_THRESH, RAMP_THRESH}; // thresholds for each section

  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X test");

  // Initialize I2C
  Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }

  Serial.println("VL53L0X ready!");

  while (!startRead) {
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }

  while (true){
    while (startRead) {
      
      VL53L0X_RangingMeasurementData_t measure;

      lox.rangingTest(&measure, false); // pass in 'true' to get debug data

      if (measure.RangeStatus != 4 && measure.RangeMilliMeter !=8191) {  // 4 means out of range
          if(measure.RangeMilliMeter<thresholds[section]  /*&& abs(measure.RangeMilliMeter - lastmeasure) > 50*/){
              portENTER_CRITICAL(&mux);
              drive = false;
              portEXIT_CRITICAL(&mux);
              Serial.println(measure.RangeMilliMeter);
              lastmeasure = measure.RangeMilliMeter;
              ++section %= 2;
          }
      }
      
    }
    Serial.println("Waiting for startRead to be true");
    delay(100);
    
  }
}