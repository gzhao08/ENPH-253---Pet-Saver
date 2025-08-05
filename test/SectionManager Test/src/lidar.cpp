#include <Arduino.h>
#include <lidar.h>
// #include <Adafruit_VL53L0X.h>
#include <VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SectionManager.h"
#include "SharedState.h"
#include "ClawManager.h"

ClawManager claw;

void objectDetected(void *parameter) {

  Serial.begin(115200);
  while (!Serial);
  // Initialize I2C
  // Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)
  
  Wire.begin(7, 5);  // SDA, SCL for right lidar 15, 13
  Wire1.begin(15, 13);  // SDA, SCL for
  
  SectionManager sectionManager;
  sectionManager.begin(false); // Initialize the section manager with display
  

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }
  
  currentSpeed = 0;
  robotState = RobotState::IDLE;

  // home claw
  claw.begin();
  claw.homingSequence();
  claw.setPositionVertical(50);
  while (!claw.vertical.reachedTarget()) {
    claw.loop();
  }
  claw.setPositionBase(0);
  Serial.println("Set position base to 0");
  while (!claw.base.reachedTarget()) {
    // Serial.println(claw.base.getPosition());
    claw.loop();
  }
  claw.stopAll();
  claw.loop();
  Serial.println("Waiting for swipe");

  // swipe to start
  while (true) {
    Serial.println(sectionManager.getMeasurement(true));
    if (sectionManager.detectCloser(true, 80, 15)) {
      currentSpeed = 1300;
      robotState = RobotState::LINE_FOLLOW;
      break;
    }
  }
  Serial.println("Starting main loop!");
  
  
  // main loop

  while(true) {

    switch (robotState) {
        case RobotState::LINE_FOLLOW: {
            // Serial.println("lidar.cpp: LINE_FOLLOW");
            sectionManager.getNextSection(); 
            break;
        }

        case RobotState::FORWARD: {
            Serial.println("lidar.cpp: FORWARD");
            sectionManager.getNextSection();
            break;
        }

        case RobotState::BACKWARD:{
          Serial.println("lidar.cpp: BACKWARD");
          sectionManager.getNextSection();
          break;
        }
        
            
        case RobotState::STOPPED: {
            Serial.println("lidar.cpp: STOPPED");
            recordStartTime();
            switch (sectionManager.getCurrentSection()) {
              case SectionManager::RAMP:{
                Serial.println("Picking up first pet");
                int petDistance = sectionManager.getMeasurement(true);
                claw.setPositionGrabber(20);  

                claw.setPositionVertical(100);
                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }
                claw.setPositionBase(-90);
                while (!claw.base.reachedTarget()) {
                  claw.loop();
                }
                claw.setPositionArm(178);
                while (!claw.arm.reachedTarget()) {
                  claw.loop();
                }

                Serial.println("Initial position of arm has been set");
                claw.base.continuousServo.logPIDOutput = true;
                claw.sensePet();

                claw.arm.moveBy(-10);
                while (!claw.arm.reachedTarget()) {
                  claw.loop();
                }

                claw.setPositionGrabber(110);

                claw.setPositionVertical(30);
                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }

                claw.setPositionGrabber(15);
                delay(500);

                claw.setPositionArm(100);
                while (!claw.arm.reachedTarget()) {
                  claw.loop();
                }
                
                claw.setPositionVertical(110);
                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }

                claw.setPositionBase(0);
                while (!claw.base.reachedTarget()) {
                  
                  claw.loop();
                }

                claw.setPositionArm(0);
                while (!claw.arm.reachedTarget()) {
                  claw.loop();
                }

                // claw.setPositionVertical(100);
                // while (!claw.vertical.reachedTarget()) {
                //   claw.loop();
                // }
        
                Serial.println("Done picking up pet, starting line follow");
                startLineFollow();
                break;


              }
              case SectionManager::WINDOW_FORWARD: {
                  startForward();
                  break;
              }
              case SectionManager::PET_3: {
                  turnCW();
                  break;
              }
              case SectionManager::RAMP_END_BACKWARD:{
                turnCW_Back();
                break;
              }
              case SectionManager::WINDOW_BACKWARD: {
                  startBackward();
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




