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

#define ACTUATE_CLAW true

void scanI2C(TwoWire &wire, const char* name);

void objectDetected(void *parameter) {

  Serial.begin(115200);
  while (!Serial);
  // Initialize I2C
  // Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)
  
  Wire.begin(7, 5);  // SDA, SCL for right lidar 15, 13
  Wire1.begin(15, 13);  // SDA, SCL for

  SectionManager sectionManager;
  sectionManager.begin(true); // Initialize the section manager with display

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }
  
  currentSpeed = 0;
  robotState = RobotState::IDLE;

  
  boolean pickedUpFirstPet = false;
  boolean droppedFirstPet = false;
  boolean pickedUpThirdPet = false;

  ClawManager claw;

  if (ACTUATE_CLAW) {
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
  }
  
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

  sectionManager.display.setCursor(30, 24);
  sectionManager.display.setTextSize(2);
  sectionManager.display.println("DOORWAY");
  sectionManager.display.display();
  delay(1000);
  Serial.println("Starting main loop!");
  
  
  // main loop
  while(true) {

    switch (robotState) {
        case RobotState::LINE_FOLLOW: {
            // Serial.println("lidar.cpp: LINE_FOLLOW");
            sectionManager.getNextSection(); 

            switch (sectionManager.getCurrentSection()) {
              case SectionManager::RAMP_END: {
                
                if (!droppedFirstPet) {
                  if (ACTUATE_CLAW) {
                    claw.clawSeq2Ramp();
                    droppedFirstPet = true;
                  }
                }
                
                break;
              }
            }
            break;
        }

        case RobotState::FORWARD: {
            // Serial.println("lidar.cpp: FORWARD");
            sectionManager.getNextSection();
            break;
        }

        case RobotState::BACKWARD:{
          Serial.println("lidar.cpp: BACKWARD");
          sectionManager.getNextSection();
          break;
        }
        
            
        case RobotState::STOPPED: {
            // Serial.println("lidar.cpp: STOPPED");
            recordStartTime();
            switch (sectionManager.getCurrentSection()) {
              case SectionManager::RAMP:{
                //PET 1
                if (!pickedUpFirstPet) {
                  if (ACTUATE_CLAW) {
                    claw.clawSeq1Pet1();
                  }
                  else {
                    delay(3000);
                  }
                }
                
                
                Serial.println("Done picking up pet, starting line follow");
                startLineFollow();
                break;
              }
              
              case SectionManager::PET_4: {
                if (!pickedUpThirdPet) {
                  //claw sequence pet 3
                  if (ACTUATE_CLAW) {
                    claw.clawSeq3Pet3();
                    pickedUpThirdPet = true;
                  }

                  else {
                    delay(3000);
                  }
                }
                startLineFollow();
                break;
              }
              
              case SectionManager::PET_2:{
                delay(3000);
                turnCW_Back();
                break;
              }

              case SectionManager::PET_5: {
                  delay(10000);
                  // turnCCW_Back();
                  break;
              }
              default:
                startLineFollow();
                break;
            }
            
            break;
        }
        
    }
    if (ACTUATE_CLAW) {
      if (sectionManager.getCurrentSection() == SectionManager::RAMP || sectionManager.getCurrentSection() == SectionManager::PET_3) {
        claw.loop();
      }
      
    }
  }  
}


void scanI2C(TwoWire &wire, const char* name) {
  Serial.print("Scanning ");
  Serial.println(name);
  
  byte count = 0;

  for (byte address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    byte error = wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0) {
    Serial.println("No I2C devices found.");
  }
  Serial.println();
}


