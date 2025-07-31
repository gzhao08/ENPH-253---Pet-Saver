#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include "SharedState.h"

#define NUM_MEASUREMENTS 500

void objectDetected(void *parameter) {

  Serial.begin(115200);
  while (!Serial);
  // Initialize I2C  
  Wire.begin(15, 13);  // SDA, SCL for right lidar
  Serial.println("Window Data Collection");

  // Lidar stuff
  Adafruit_VL53L0X rightLidar = Adafruit_VL53L0X();
  VL53L0X_RangingMeasurementData_t rightMeasure;
  Serial.println("VL53L0X test");
  Serial.println("Initializing Right VL53L0X...");
  while (!rightLidar.begin( (uint8_t) 41U, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot Right VL53L0X, retrying...");
    delay(100);
  }


  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }
  
  startDrive();

  Serial.println("Motors On");

  int* data = (int*) malloc(NUM_MEASUREMENTS*sizeof(int)); // Allocate memory for data collection
  int dataIndex = 0; // Index for data collection

  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    data[i] = 0; // Initialize data array
  }

  Serial.println("Starting data collection...");

  while(dataIndex < NUM_MEASUREMENTS) {
    rightLidar.rangingTest(&rightMeasure, false);
    if (rightMeasure.RangeStatus != 4) {
      data[dataIndex++] = rightMeasure.RangeMilliMeter;
    }
  }

  Serial.println("Data collection complete.");

  Serial.println("Printing Data.");

  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    Serial.print(i);
    Serial.print(",");
    Serial.println(data[i]);
  }

  Serial.println("Data printed successfully.");

  while(true) {
    delay(10000);
  }
  
}




