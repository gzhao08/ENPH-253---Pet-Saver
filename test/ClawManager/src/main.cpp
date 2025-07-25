#include <Arduino.h>
#include <Wire.h>

#include "managers/WireManager.h"
#include "managers/DelayManager.h"
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
#include "sensors/Microswitch.h"


WireManager wireManager(8); 
ClawGrabber grab(22, 1);


int motorPin1 = 2;
int motorPin2 = 12;
int pwmChannel1 = 2; //motor pin 1 goes to B
int pwmChannel2 = 3; 
int muxLine = 1; 
bool encoderOnTerminalSide = false; //false for arm, 
int switchPin = 37; 
bool normallyOpen = true; 

DelayManager positionDelayManager(1000);
//int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin
//mux: 1 is 1, 0 is 2, -1 is not muxing
ClawArm arm(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide, switchPin, normallyOpen); 

void setup() {
  Serial.begin(115200);
  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(15, 13);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  // 3. Begin servo
  arm.begin(&wireManager); 
  positionDelayManager.reset();

  //arm.home();

  grab.begin();
  
  
}

unsigned long lastUpdate = 0;
bool home = true; 
bool ten = true;
int position = -10;

void loop() {

  // if (home) {
  //   arm.home();
  // }

  // home = false; 

  // if (ten) {
  //     arm.setPosition(10);
  // }
  // else {
  //      arm.setPosition(50);
  // }

  // arm.loop();
   
  // if (positionDelayManager.checkAndReset()) {
  //   if (ten) {
  //     ten = false;
  //   } else {
  //     ten = true;
  //   }
    
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


/** test grabber */
grab.setPositionDutyCycle(grab.PET_CLOSE);
delay(2000);
grab.setPositionDutyCycle(grab.PARALLEL); //parallel from closed
delay(500);
grab.setPositionDutyCycle(grab.OPEN);
delay(500);
grab.setPositionDutyCycle(10); //parallel from open
delay(5000);

 
  
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