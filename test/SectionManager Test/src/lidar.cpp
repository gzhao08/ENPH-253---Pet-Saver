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
  
  currentSpeed = 1300;
  startLineFollow();
  int thresholds[6] = {250, 250, 250, 350, 325, 350}; // thresholds for sections 0-5
  int stops[6] = {false, false, true, true, true, true};
  int useRightLidar[6] = {true, true, true, true, true, false};
  int consecutiveCount[] = {2, 2, 2, 2, 15, 2}; // number of consecutive measurements to consider a section change

  
  
  while(true) {

    switch (robotState) {
        case RobotState::LINE_FOLLOW: {
            Serial.println("lidar.cpp: LINE_FOLLOW");
            sectionManager.getNextSection();
            break;
        }

        case RobotState::FORWARD: {
            Serial.println("lidar.cpp: FORWARD");
            sectionManager.getNextSection();
            break;
        }
            
        case RobotState::STOPPED: {
            Serial.println("lidar.cpp: STOPPED");
            delay(3000);
            recordStartTime();
            switch (sectionManager.getCurrentSection()) {
              case SectionManager::WINDOW_FORWARD: {
                  startForward();
                  break;
              }
              case SectionManager::PET_3: {
                  turnCW();
                  break;
              }
              case SectionManager::WINDOW_BACKWARD: {
                  turnCW_Back();
                  break;
              }
              default:
                startLineFollow();
                break;
            }
            
            break;
        }
    }  
  delay(10);
  }  
}




