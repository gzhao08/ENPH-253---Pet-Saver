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

int armMotorPin1 = ARM_MOTOR_PIN_1;
int armMotorPin2 = ARM_MOTOR_PIN_2;
int armPwmChannel1 = ARM_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int armPwmChannel2 = ARM_MOTOR_PWM_CHANNEL_2; 
int armMuxLine = 1; 
bool armEncoderOnTerminalSide = true; //false for arm, 
int armSwitchPin = MICROSWITCH_ARM; 
bool armNormallyOpen = true; 
//muxLine: 1 is 1, 0 is 2, -1 is not muxing

int verticalStageMotorPin1 = VERTICAL_STAGE_MOTOR_PIN_1;
int verticalStageMotorPin2 = VERTICAL_STAGE_MOTOR_PIN_2;
int verticalStagePwmChannel1 = VERTICAL_STAGE_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int verticalStagePwmChannel2 = VERTICAL_STAGE_MOTOR_PWM_CHANNEL_2; 
int verticalStageMuxLine = 0; 
bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
int verticalStageSwitchPin = MICROSWITCH_VERTICAL_STAGE; 
bool verticalStageNormallyOpen = true; 

int baseMotorPin1 = BASE_MOTOR_PIN_1;
int baseMotorPin2 = BASE_MOTOR_PIN_2;
int basePwmChannel1 = BASE_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int basePwmChannel2 = BASE_MOTOR_PWM_CHANNEL_2; 
int baseMuxLine = -1; 
bool baseEncoderOnTerminalSide = false; //false for arm, 
int baseSwitchPin = MICROSWITCH_BASE; 
bool baseNormallyOpen = true; 

ClawManager claw(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
  armSwitchPin, armNormallyOpen, verticalStageMotorPin1, verticalStageMotorPin2, verticalStagePwmChannel1, verticalStagePwmChannel2, verticalStageMuxLine, verticalStageEncoderOnTerminalSide,
  verticalStageSwitchPin, verticalStageNormallyOpen, baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, 
  baseSwitchPin, baseNormallyOpen, GRABBER_MOTOR_PIN, GRABBER_PWM_CHANNEL, 8, -1);

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
  
  currentSpeed = 1300;
  startLineFollow();
  int thresholds[6] = {250, 250, 250, 350, 325, 350}; // thresholds for sections 0-5
  int stops[6] = {false, false, true, true, true, true};
  int useRightLidar[6] = {true, true, true, true, true, false};
  int consecutiveCount[] = {2, 2, 2, 2, 15, 2}; // number of consecutive measurements to consider a section change

  
  
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
                Serial.println("Picking up pet");
                int petDistance = sectionManager.getMeasurement(true);
                claw.begin();
                claw.homingSequence();
                claw.setPositionGrabber(110);
                claw.setPositionVertical(100);

                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }
                claw.setPositionBase(-90);
                while (!claw.base.reachedTarget()) {
                  claw.loop();
                }
                claw.setPositionArm(200);
                while (!claw.arm.reachedTarget()) {
                  claw.loop();
                }

                claw.sensePet();


                claw.setPositionVertical(60);
                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }
                claw.setPositionGrabber(20);
                delay(500);
                claw.setPositionVertical(120);
                while (!claw.vertical.reachedTarget()) {
                  claw.loop();
                }
        
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




