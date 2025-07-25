#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"



void objectDetected(void *parameter) {
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  int lastmeasure = 0;
  int section = 0;
  int num_consecutive = 0; // number of consecutive measurements below threshold
  int thresholds[2] = {DOORWAY_THRESH, RAMP_THRESH}; // thresholds for each section

  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X test");

  // Initialize I2C
  Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)

  while (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    delay(100);
  }

  lox.setMeasurementTimingBudgetMicroSeconds(20000);
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
              num_consecutive++;
              if (num_consecutive >= 3) { // if we have 3 consecutive measurements below the threshold
                portENTER_CRITICAL(&mux);
                drive = false;
                portEXIT_CRITICAL(&mux);
                ++section %= 2;
                num_consecutive = 0;
              }
              Serial.println(measure.RangeMilliMeter);
              //lastmeasure = measure.RangeMilliMeter;
          }
      }
      
    }
    Serial.println("Waiting for startRead to be true");
    delay(100);
    
  }
}