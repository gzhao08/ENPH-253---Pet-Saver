#include <Arduino.h>
#include <Wire.h>

#include "managers/WireManager.h"
#include "managers/DelayManager.h"
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
#include "claw/ClawVerticalStage.h"
#include "claw/ClawBase.h"

#include "sensors/Microswitch.h"
#include "GlobalConstants.h"

MyServo servo(12, 1);
ClawGrabber grabber(12, 1);

int muxPin = 8;
WireManager wireManager(muxPin); 
WireManager wireManager2(-1); 


DelayManager positionDelayManager(4000);


int armMotorPin1 = 22;
int armMotorPin2 = 19;
int armPwmChannel1 = 15; //motor pin 1 goes to B
int armPwmChannel2 = 14; 
int armMuxLine = 1; 
bool armEncoderOnTerminalSide = true; //false for arm, 
int armSwitchPin = 37; 
bool armNormallyOpen = true; 
//muxLine: 1 is 1, 0 is 2, -1 is not muxing
ClawArm arm(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
  armSwitchPin, armNormallyOpen); 

int verticalStageMotorPin1 = 20;
int verticalStageMotorPin2 = 21;
int verticalStagePwmChannel1 = 4; //motor pin 1 goes to B
int verticalStagePwmChannel2 = 5; 
int verticalStageMuxLine = 0; 
bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
int verticalStageSwitchPin = 38; 
bool verticalStageNormallyOpen = true; 

ClawVerticalStage verticalStage(verticalStageMotorPin1, verticalStageMotorPin2, verticalStagePwmChannel1, verticalStagePwmChannel2, verticalStageMuxLine, verticalStageEncoderOnTerminalSide,
  verticalStageSwitchPin, verticalStageNormallyOpen);

int baseMotorPin1 = 14;
int baseMotorPin2 = 27;
int basePwmChannel1 = 6; //motor pin 1 goes to B
int basePwmChannel2 = 7; 
int baseMuxLine = -1; 
bool baseEncoderOnTerminalSide = false; //false for arm, 
int baseSwitchPin = 9; 
bool baseNormallyOpen = true; 

ClawBase base(baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, baseSwitchPin, baseNormallyOpen);

void setup() {
  Serial.begin(115200);\
  Serial.setTimeout(3000);
  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(5, 7);
  Wire1.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  wireManager2.begin(&Wire1);
  // 3. Begin servo
  arm.begin(&wireManager); 
  verticalStage.begin(&wireManager);
  base.begin(&wireManager2);
  servo.begin();
  grabber.begin();

  // base.continuousServo.logPIDOutput = true;
  // positionDelayManager.reset();

  // arm.continuousServo.logPIDOutput = true;
  arm.setPosition(100);

}

unsigned long lastUpdate = 0;
bool ten = true;

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    int intCode = line.toInt();

    switch (intCode) {
      case 1: // Check arm
        while (true) {
          Serial.println("Arm position: " + String(arm.getPosition()));
          arm.loop();

          if (Serial.available()) {
            line = Serial.readStringUntil('\n');
            Serial.println("Quit");
            break;
          }
        }
        break;
      
      case 2:
        while (true) {
          Serial.println("Vertical stage position: " + String(verticalStage.getPosition()));
          verticalStage.loop();

          if (Serial.available()) {
            line = Serial.readStringUntil('\n');
            Serial.println("Quit");

            break;
          }
        }
        break;


      case 3:
        while (true) {
          Serial.println("Base position: " + String(base.getPosition()));
          base.loop();

          if (Serial.available()) {
            line = Serial.readStringUntil('\n');
            Serial.println("Quit");
            break;
          }
        }
        break;

      
      case 4:
        Serial.println("Command: Arm homing");
        arm.homingSequence();
        arm.setPosition(100);
        break;
      case 5:
        Serial.println("Command: Vertical homing");
        verticalStage.homingSequence();
        break;
      case 6:
        Serial.println("Command: Base homing");
        base.homingSequence();
        break;

      case 7:
        Serial.println("Command: Arm test sequence");
        arm.testSequence();
        break;
      case 8:
        Serial.println("Command: Vertical test sequence");
        verticalStage.testSequence();
        break;
      case 9:
        Serial.println("Command: Base test sequence");
        base.testSequence();
        break;
      
      case 10:
        Serial.println("Command: Arm set position");
        while (true) {
          arm.loop();
          if (Serial.available()) {
            String pos = Serial.readStringUntil('\n');
            if (pos.toInt() != -1) {
              arm.setPosition(pos.toInt());
              Serial.println("Set position  of arm to: " + String(pos.toInt()));
            } else {
              Serial.println("Quit");
              break;
            }
          }
        }
        break;
      case 11:
        Serial.println("Command: Vertical set position");
        while (true) {
          verticalStage.loop();
          if (Serial.available()) {
            String pos = Serial.readStringUntil('\n');
            if (pos.toInt() != -1) {
              verticalStage.setPosition(pos.toInt());
              Serial.println("Set position of vertical stage to: " + String(pos.toInt()));
            } else {
              break;
            }
          }
        }
        break;
      case 12:
        Serial.println("Command: Base set position");
        while (true) {
          base.loop();
          if (Serial.available()) {
            String pos = Serial.readStringUntil('\n');
            if (pos.toInt() != -1) {
              base.setPosition(pos.toInt());
              Serial.println("Set position of base to: " + String(pos.toInt()));
            } else {
              break;
            }
          }
        }
        break;
      
      case 13:
        Serial.println("Command: Grabber close");
        grabber.close();
        break;
      case 14:
        Serial.println("Command: Grabber open");
        grabber.open();
        break;
      case 15:
        Serial.println("Command: Grabber parallel");
        grabber.parallel();
        break;
      case 16:
        Serial.println("Command: Grabber set position");
        while (true) {
          if (Serial.available()) {
            String pos = Serial.readStringUntil('\n');
            if (pos.toInt() != -1) {
              grabber.setPositionDegrees(pos.toInt());
              Serial.println("Set position of grabber to: " + String(pos.toInt()));
            } else {
              break;
            }
          }
      case 17:
      {
        Serial.println("Command: PID Tuning arm");
        DelayManager positionDelay(2000);
        positionDelay.reset();
        const int posArm1 = 50;
        const int posArm2 = 150;
        int posArm = posArm1;
        while (true) {
          if (Serial.available()) {
            String Pks = Serial.readStringUntil('\n');
            float Pk = Pks.toFloat();
            String Dks = Serial.readStringUntil('\n');
            float Dk = Dks.toFloat();
            if (Dk == -1 || Pk == -1) {
              Serial.println("Quit");
              break;
            } else {
              Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
              arm.continuousServo.setPDTuning(Pk, Dk);
            }
          }

          arm.loop();

          if(positionDelay.checkAndReset()) {
            if (posArm == posArm1) {
              posArm = posArm2;
            } else {
              posArm = posArm1;
            }
            Serial.printf("Arm position: %.3f\n", arm.getPosition());
            arm.setPosition(posArm);
          }

        }
        break;
      }
      case 18:
      {
        Serial.println("Command: PID Tuning vertical");        DelayManager positionDelay(2000);
        positionDelay.reset();
        const int posVert1 = 30;
        const int posVert2 = 70;
        int posVert = posVert1;
        while (true) {
          if (Serial.available()) {
            String Pks = Serial.readStringUntil('\n');
            float Pk = Pks.toFloat();
            String Dks = Serial.readStringUntil('\n');
            float Dk = Dks.toFloat();
            if (Dk == -1 || Pk == -1) {
              Serial.println("Quit");
              break;
            } else {
              Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
              verticalStage.continuousServo.setPDTuning(Pk, Dk);
            }
          }

          verticalStage.loop();

          if(positionDelay.checkAndReset()) {
            if (posVert == posVert1) {
              posVert = posVert2;
            } else {
              posVert = posVert1;
            }
            Serial.printf("Vertical position: %.3f\n", verticalStage.getPosition());
            verticalStage.setPosition(posVert);
          }
        }
        break;
      }
      case 19:
      {
        Serial.println("Command: PID Tuning base");
        DelayManager positionDelay(2000);
        positionDelay.reset();
        const int posBase1 = -30;
        const int posBase2 = 30;
        int posBase = posBase1;
        while (true) {
          if (Serial.available()) {
            String Pks = Serial.readStringUntil('\n');
            float Pk = Pks.toFloat();
            String Dks = Serial.readStringUntil('\n');
            float Dk = Dks.toFloat();
            if (Dk == -1 || Pk == -1) {
              Serial.println("Quit");
              break;
            } else {
              Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
              base.continuousServo.setPDTuning(Pk, Dk);
            }
          }

          base.loop();

          if(positionDelay.checkAndReset()) {
            if (posBase == posBase1) {
              posBase = posBase2;
            } else {
              posBase = posBase1;
            }
            Serial.printf("Base position: %.3f\n", base.getPosition());
            base.setPosition(posBase);
          }
        }
        break;
      }
    }
  }
}

// test grabber 
  // grab.setPositionDutyCycle(grab.PET_CLOSE);
  // Serial.println("CLOSED");
  // delay(2000);
  // // grab.setPositionDutyCycle(grab.PARALLEL); //parallel from closed
  // // delay(500);
  // grab.setPositionDutyCycle(grab.OPEN);
  // delay(500);
  // delay(500);
  // grab.setPositionDutyCycle(10); //parallel from open
  // delay(5000);

 
  
// test servo
  // servo.writePosition(0);
  // delay(2000);
  // servo.writePosition(40);
  // delay(500);
  // servo.writePosition(60);
  // delay(500);
  // servo.writePosition(80);
  // delay(500);
  // servo.writePosition(100);
  // delay(500);
  // servo.writePosition(120);
  // delay(500); 
  // servo.writePosition(140);
  // delay(500);
  // servo.writePosition(160);
  // delay(500);
  // servo.writePosition(180);
  // delay(500);

}