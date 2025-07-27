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

// MyServo servoMotor(22, 1);
ClawGrabber grab(22, 1);

int muxPin = 8;
WireManager wireManager(muxPin); 
WireManager wireManager2(-1); 


DelayManager positionDelayManager(4000);


int armMotorPin1 = 22;
int armMotorPin2 = 19;
int armPwmChannel1 = 2; //motor pin 1 goes to B
int armPwmChannel2 = 3; 
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
bool baseEncoderOnTerminalSide = true; //false for arm, 
int baseSwitchPin = 9; 
bool baseNormallyOpen = true; 

ClawBase base(baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, baseSwitchPin, baseNormallyOpen);


void setup() {
  Serial.begin(115200);
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

  Serial.println("JESUS PLS WORK");
  // positionDelayManager.reset();


  // grab.begin();
  
  // arm.homingSequence();
  // arm.testSequence();

  // base.homingSequence();
  // verticalStage.homingSequence();

}

unsigned long lastUpdate = 0;
bool ten = true;

void loop() {
  // Serial.println(arm.getPosition());
  // Serial.println(verticalStage.getPosition());
  if (ten) {
      arm.setPosition(10);
      verticalStage.setPosition(10);

  } else {
      arm.setPosition(50);
      verticalStage.setPosition(50);
  }

  // arm.loop();
  // verticalStage.loop();
  // Serial.println(base.getPosition());
  base.loop();
  if (positionDelayManager.checkAndReset()) {
    if (ten) {
      ten = false;
    } else {
      ten = true;
    }
  }

 
  // arm.setPosition(20);
  // arm.loop();
  // delay(500);
  // //arm.setPosition(20);
  // arm.loop();
  // delay(500);

  // Serial.println("WOI KENAPA GABSIA");
/** test arm */
  //arm.home();
  //arm.loop();
//arm.loop();
// arm.setPosition(10);
// arm.loop();
// delay(1000);
// arm.setPosition(20);
// arm.loop();
// delay(1000);


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
  // delay(500);
  // servo.writePosition(10);
  // delay(500);
  // servo.writePosition(20);
  // delay(500);
  // servo.writePosition(30);
  // delay(500);
  // servo.writePosition(40);
  // delay(500);
  // servo.writePosition(50);
  // delay(500);
  // servo.writePosition(60);
  // delay(500);
  // servo.writePosition(70);
  // delay(500);
  // servo.writePosition(80);
  // delay(500);
  // servo.writePosition(90);
  // delay(500);
  // servo.writePosition(100);
  // delay(500);
  // servo.writePosition(110);
  // delay(500);
  // servo.writePosition(120);
  // delay(500); 
  // servo.writePosition(130);
  // delay(500);
  // servo.writePosition(140);
  // delay(500);
  // servo.writePosition(150);
  // delay(500);
  // servo.writePosition(160);
  // delay(500);
  // servo.writePosition(170);
  // delay(500);
  // servo.writePosition(180);
  // delay(500);

}