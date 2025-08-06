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
  sectionManager.begin(true); // Initialize the section manager with display
  

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
  boolean droppedFirstPet = false;
  boolean pickedUpThirdPet = false;
  while(true) {

    switch (robotState) {
        case RobotState::LINE_FOLLOW: {
            // Serial.println("lidar.cpp: LINE_FOLLOW");
            sectionManager.getNextSection(); 

            switch (sectionManager.getCurrentSection()) {
              case SectionManager::RAMP_END: {
                if (!droppedFirstPet) {
                  // Move to drop position
                  claw.base.setPosition(90); //left
                  claw.arm.setPosition(150); //out
                  claw.waitToReachTarget(3000);
                  // Open the claw
                  Serial.println("Claw: 90 base 100 arm");
                  claw.setPositionGrabber(100);  //open
                  Serial.println("Grabber opened");
                  delay(100); // Wait for grabber to open
                  // Pull arm in 
                  droppedFirstPet = true;
                  claw.arm.setPosition(0); //in
                  claw.base.setPosition(70); //forward
                  claw.waitToReachTarget(3000);
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
                Serial.println("Picking up first pet");
                int petDistance = sectionManager.getMeasurement(true);
                // Open grabber
                claw.setPositionGrabber(110);  

                // Move to sensing position
                claw.setPositionBase(-90);
                claw.setPositionArm(178);
                claw.setPositionVertical(10);
                claw.waitToReachTarget();
                
                Serial.println("Initial position of arm has been set");
                // claw.base.continuousServo.logPIDOutput = true;
                claw.sensePet();

                claw.arm.moveBy(65);
                claw.waitToReachTarget(3000);

                // Let it grab
                claw.setPositionGrabber(10);
                delay(500);

                // Position while driving up ramp
                claw.setPositionArm(65); //in
                claw.setPositionVertical(80); //up
                claw.setPositionBase(0); //forward
                claw.waitToReachTarget(2000);
        
                Serial.println("Done picking up pet, starting line follow");
                startLineFollow();
                break;
              }
              
              case SectionManager::PET_4: {
                if (!pickedUpThirdPet) {
                  //claw sequence pet 3
                  // Make sure arm in and home in to pillar
                  claw.arm.setPosition(0);
                  claw.base.setPosition(60);
                  claw.vertical.setPosition(140); //check if high enough
                  claw.waitToReachTarget(5000);

                  // Extend the arm and sense the pet
                  claw.arm.setPosition(150);
                  claw.waitToReachTarget(5000);
                  claw.sensePet();

                  // Pick up pet
                  claw.setPositionGrabber(20);
                  delay(300);

                  // Move to basket position
                  claw.arm.setPosition(0);
                  claw.base.setPosition(-20);
                  claw.waitToReachTarget(2000);

                  // Lowers and drops to basket
                  claw.vertical.setPosition(80);
                  claw.waitToReachTarget(2000);
                  claw.setPositionGrabber(110);
                  pickedUpThirdPet = true;
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
    claw.loop();
  }  
}




