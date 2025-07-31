#include <Arduino.h>
#include <lidar.h>
// #include <Adafruit_VL53L0X.h>
#include <VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SectionManager.h"
#include "SharedState.h"

void objectDetected(void *parameter) {

  Serial.begin(115200);
  while (!Serial);
  // Initialize I2C
  // Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)
  
  Wire.begin(15, 13);  // SDA, SCL for right lidar
  Wire1.begin(7, 5);  // SDA, SCL for
  
  SectionManager sectionManager;
  sectionManager.begin(true); // Initialize the section manager with display
  

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }
  
  currentSpeed = 500;
  startDrive();
  int thresholds[6] = {250, 250, 250, 350, 325, 350}; // thresholds for sections 0-5
  int stops[6] = {false, false, true, true, true, true};
  int useRightLidar[6] = {true, true, true, true, true, false};
  int consecutiveCount[] = {2, 2, 2, 2, 15, 2}; // number of consecutive measurements to consider a section change

  
  
  while(true) {
    // drive and use lidar to stop
    // startDrive();
    // sectionManager.detectCloser(useRightLidar[0], thresholds[0], consecutiveCount[0]);
    // stopDrive();
    // sectionManager.show("STOP");
    // delay(2000);
    // sectionManager.detectOutOfRange(useRightLidar[0]);

    startDrive();
    switch(sectionManager.getCurrentSection()) {
      case SectionManager::DOORWAY: {
        // looking for doorway
        if (sectionManager.detectCloser(false, 120, 1)) {
          sectionManager.incrementSection();
          stopDrive();
          sectionManager.show("STOP");
          delay(1000);
        }
        break;
      }

      case SectionManager::PET_1: {
        if (sectionManager.detectCloser(true, 200, 3)) {
          sectionManager.incrementSection();
          stopDrive();
          sectionManager.show("STOP");
          delay(1000);
        }
        break;
      }
    }
    
  }
  

  
}




