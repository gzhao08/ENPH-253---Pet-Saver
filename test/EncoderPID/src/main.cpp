#include <Arduino.h>
#include <Wire.h>

// Local Libraries:
#include "GlobalConstants.h"
#include "sensors/MagneticEncoder.h"
#include "motors/ContinuousServo.h"
#include "managers/WireManager.h"

#define AS5600_ADDR 0x36

// When IN1 is HIGH, it is like when M1 is connected to Supply
#define motor_IN1 2
#define motor_IN2 12

#define pwmChannel1 1
#define pwmChannel2 2

#define P_Pin 34
#define D_Pin 35

// PID Control
double Pk1 = 180; 
double Ik1 = 0;
double Dk1 = 5;

double Setpoint = 0, Input = 0, Output = 0;

// int microswitchPin = 34;

WireManager wireManager(8);
// MagneticEncoder encoder(0);
// MagneticEncoder encoder2(1);

ContinuousServo servo(motor_IN1, motor_IN2, pwmChannel1, pwmChannel2, 1, true);

void homingSequence();

void setup() {
    Serial.begin(115200);
  // Flow:

  // 1. Initialize Wire
  Wire.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);
  Wire.setClock(100000);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  // 3. Begin servo
  servo.begin(&wireManager);

  servo.setPIDTuningMode(true);
  servo.setPIDTuningPins(P_Pin, D_Pin);

  // pinMode(microswitchPin, INPUT);

  // homingSequence();

}



void loop() {
    servo.testSequence();
    Serial.println("Test sequence done");
    

  // if (Serial.available()) {
  //   String line = Serial.readStringUntil('\n');
  //   int movement = line.toInt();
  //   Serial.printf("moveBy: %d\n", movement);
  //   servo.moveBy(movement);
  // }
  // servo.loop();
  // Serial.println(encoder.readAngle());
  // int time = millis() % 2500;
  // int targetAngle;
  // if (time < 500) {
  //   targetAngle = 0;
  // } else if (time < 1000) {
  //   targetAngle = 90;
  // } else if (time < 1500) {
  //   targetAngle = 180;
  // } else if (time < 2000) {
  //   targetAngle = 270;
  // } else {
  //   targetAngle = 20;
  // }
}

// void homingSequence() {
//   Serial.println(digitalRead(microswitchPin));
//   while(digitalRead(microswitchPin) != HIGH) {
//     servo.drivePWM(-800);
//     servo.loop();

//   }
//   servo.homingSequence();

//   servo.moveTo(-100);
//   while (!servo.reachedTarget()) {
//     servo.loop();
//   }
//   Serial.println("Bounced");

//   // servo.moveBy(20);
//   // while (!servo.reachedTarget()) {
//   //   servo.loop();
//   // }

//   // Serial.println("Reached target");
  
  
//   // servo.moveBy(10);
//   // while (!servo.reachedTarget()) {
//   //   servo.loop();
//   // }
// }
