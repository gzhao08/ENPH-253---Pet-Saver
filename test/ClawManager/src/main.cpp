#include <Arduino.h>
#include <Wire.h>

#include "managers/WireManager.h"
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
#include "sensors/Microswitch.h"


WireManager wireManager(8); 
ClawGrabber grab(22, 1);


int motorPin1 = 2;
int motorPin2 = 12;
int pwmChannel1 = 2;
int pwmChannel2 = 3; 
int muxLine = 1; 
bool encoderOnTerminalSide = false; 
int switchPin = 37; 
bool normallyOpen = true; 

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

  //arm.home();

  grab.begin();
  
}

void loop() {
  arm.setPosition(10);
  // arm.loop();

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