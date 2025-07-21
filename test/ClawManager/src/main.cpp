#include <Arduino.h>
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
//#include "../../../objects/sensors/Microswitch"

ClawGrabber grab(22, 1);
ClawArm arm(); 



void setup() {
  grab.begin(); 
}

void loop() {

/** test arm */
arm->setPosition(10);
delay(1000);
arm->setPosition(20);

/** test grabber
grab->setPositionDutyCycle(grab->PET_CLOSE);
delay(5000);
grab->setPositionDutyCycle(grab->PARALLEL);
delay(500);
grab->setPositionDutyCycle(grab->OPEN);
delay(500);
 */
  
/** test servo
  servo->writePosition(0);
  delay(500);
  servo->writePosition(10);
  delay(500);
  servo->writePosition(20);
  delay(500);
  servo->writePosition(30);
  delay(500);
  servo->writePosition(40);
  delay(500);
  servo->writePosition(50);
  delay(500);
  servo->writePosition(60);
  delay(500);
  servo->writePosition(70);
  delay(500);
  servo->writePosition(80);
  delay(500);
  servo->writePosition(90);
  delay(500);
  servo->writePosition(100);
  delay(500);
  servo->writePosition(110);
  delay(500);
  servo->writePosition(120);
  delay(500); 
  servo->writePosition(130);
  delay(500);
  servo->writePosition(140);
  delay(500);
  servo->writePosition(150);
  delay(500);
  servo->writePosition(160);
  delay(500);
  servo->writePosition(170);
  delay(500);
  servo->writePosition(180);
  delay(500);
*/ 

}