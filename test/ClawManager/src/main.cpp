#include <Arduino.h>
#include <Wire.h>

#include "managers/WireManager.h"
#include "managers/DelayManager.h"
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
#include "sensors/Microswitch.h"
#include "GlobalConstants.h"


ClawGrabber grab(22, 1);

int muxPin = 8;
WireManager wireManager(muxPin); 

int armMotorPin1 = 2;
int armMotorPin2 = 12;
int armPwmChannel1 = 2; //motor pin 1 goes to B
int armPwmChannel2 = 3; 
int armMuxLine = 1; 
bool armEncoderOnTerminalSide = true; //false for arm, 
int armSwitchPin = 38; 
bool armNormallyOpen = true; 

DelayManager positionDelayManager(4000);
//muxLine: 1 is 1, 0 is 2, -1 is not muxing
ClawArm arm(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
  armSwitchPin, armNormallyOpen); 

void setup() {
  Serial.begin(115200);
  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  // 3. Begin servo
  arm.begin(&wireManager); 
  positionDelayManager.reset();

  //arm.home();

  grab.begin();
  
  arm.homingSequence();
  arm.testSequence();
}

unsigned long lastUpdate = 0;
bool ten = true;

void loop() {
  if (ten) {
      arm.setPosition(10);
  } else {
       arm.setPosition(100);
  }

  arm.loop();
   
  if (positionDelayManager.checkAndReset()) {
    if (ten) {
      ten = false;
    } else {
      ten = true;
    }
  }
  
  //   // home = false; 
  //   // position -= 10; 
  //   // if (position = -50) {
  //   //   position = 0;
  //   // }
  // }

 
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


/** test grabber 
grab.setPositionDutyCycle(grab.PET_CLOSE);
delay(2000);
grab.setPositionDutyCycle(grab.PARALLEL); //parallel from closed
delay(500);
grab.setPositionDutyCycle(grab.OPEN);
delay(500);
grab.setPositionDutyCycle(10); //parallel from open
delay(5000);
*/
 
  
/** test servo
  servo.writePosition(0);
  delay(500);
  servo.writePosition(10);
  delay(500);
  servo.writePosition(20);
  delay(500);
  servo.writePosition(30);
  delay(500);
  servo.writePosition(40);
  delay(500);
  servo.writePosition(50);
  delay(500);
  servo.writePosition(60);
  delay(500);
  servo.writePosition(70);
  delay(500);
  servo.writePosition(80);
  delay(500);
  servo.writePosition(90);
  delay(500);
  servo.writePosition(100);
  delay(500);
  servo.writePosition(110);
  delay(500);
  servo.writePosition(120);
  delay(500); 
  servo.writePosition(130);
  delay(500);
  servo.writePosition(140);
  delay(500);
  servo.writePosition(150);
  delay(500);
  servo.writePosition(160);
  delay(500);
  servo.writePosition(170);
  delay(500);
  servo.writePosition(180);
  delay(500);
*/ 

}