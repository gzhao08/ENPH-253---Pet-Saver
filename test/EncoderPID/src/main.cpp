#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "driver/ledc.h"

#include "MagneticEncoder.h"
#include "ContinuousServo.h"

#define AS5600_ADDR 0x36
#include <AS5600.h>

#define I2C_SCL_1 10
#define I2C_SDA_1 9

// When IN1 is HIGH, it is like when M1 is connected to Supply
#define motor_IN1 32 
#define motor_IN2 33
#define pwmChannel1 1
#define pwmChannel2 2

#define BIT_12_LIMIT 4096
#define P_Pin 34
#define D_Pin 38

// PID Control
double Pk1 = 180; 
double Ik1 = 0;
double Dk1 = 5;

double Setpoint = 0, Input = 0, Output = 0;

MagneticEncoder encoder;
ContinuousServo servo(motor_IN1, motor_IN2, pwmChannel1, pwmChannel2);

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_1, I2C_SCL_1);

  encoder.begin(&Wire);
  servo.begin(&encoder);
  // servo.testSequence();

}



void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    int movement = line.toInt();
    Serial.printf("moveBy: %d\n", movement);
    servo.moveBy(movement);
  }
  servo.loop();
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

/**
 * dir 0: CCW irt encoder
 * dir 1: CW irt encoder
 * else, stop
 * Pins must be driven low before high to prevent shoot through.
 */
void runMotor(int dir) {
  if (dir == 0) {
    ledcWrite(pwmChannel2, 0);
    ledcWrite(pwmChannel1, BIT_12_LIMIT);
  } else if (dir == 1) {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, BIT_12_LIMIT);
  } else {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, 0);
  }
}
