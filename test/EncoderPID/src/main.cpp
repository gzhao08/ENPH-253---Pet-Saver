#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "driver/ledc.h"

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
PID myPID(&Input, &Output, &Setpoint, Pk1, Ik1, Dk1, DIRECT);

// put function declarations here:
void runMotor(int);
float computeAngleError(float, float);
void driveMotorPID(double);

/**
 * Read raw angle from sensor
 * WireObj depends on which I2C channel is used
 */
uint16_t readRawAngle(TwoWire& WireObj) {
  WireObj.beginTransmission(AS5600_ADDR);
  WireObj.write(0x0C);  // RAW ANGLE MSB (0x0D for LSB, autoincrement)

  if (WireObj.endTransmission(false) != 0) { 
    return 0xFFFF; // Transmission failed
  }

  WireObj.requestFrom(AS5600_ADDR, 2); // Request MSB and LSB

  if (WireObj.available() == 2) {
    uint8_t msb = WireObj.read();
    uint8_t lsb = WireObj.read();
    return ((uint16_t) msb << 8) | lsb;
  }

  return 0xFFFF;
}

/**
 * Reads angle 
 * WireObj depends on which I2C channel is used
 * Output range: 0 to 360
 * Something is wrong, Output = -1 
 */
double readAngle(TwoWire& WireObj) {
  uint16_t angle = readRawAngle(WireObj);
  if (angle != 0xFFFF) {
          Serial.println((angle * 360.0) / 4096.0);
    return (angle * 360.0) / 4096.0;
  } else {
    return -1;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_1, I2C_SCL_1);

  myPID.SetMode(AUTOMATIC);              // PID Setup
  myPID.SetOutputLimits(-BIT_12_LIMIT, BIT_12_LIMIT);
  myPID.SetSampleTime(20);

  ledcSetup(pwmChannel1, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(motor_IN1, pwmChannel1);

  ledcSetup(pwmChannel2, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(motor_IN2, pwmChannel2);
}



void loop() {
    Serial.println("HELLO");

  int time = millis() % 2500;
  int targetAngle;
  if (time < 500) {
    targetAngle = 0;
  } else if (time < 1000) {
    targetAngle = 90;
  } else if (time < 1500) {
    targetAngle = 180;
  } else if (time < 2000) {
    targetAngle = 270;
  } else {
    targetAngle = 20;
  }

  // PID Tuning
  // int newKp = map(analogRead(P_Pin), 0, BIT_12_LIMIT, 0, 1000);
  // double newKd = ((double) map(analogRead(D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;
  // Serial.print("Kp: ");
  // Serial.println(newKp);
  // Serial.print("Kd: ");
  // Serial.println(newKd);
  // myPID.SetTunings(newKp, 0, newKd);

  double angle = readAngle(Wire);

  if (angle != -1) {
    // PID Feedback
    float angleError = computeAngleError(targetAngle, angle);
    Input = angleError;
    myPID.Compute();
    driveMotorPID(Output);
  
    Serial.print(angle, 2);
    Serial.println(" deg");

    Serial.print("Angle error: ");
    Serial.println(angleError);
    Serial.print("PID Output: ");
    Serial.println(Output);
  } else {
    Serial.println("No AS5600 detected.");
  }


  Serial.println("-----");
  delay(10);
}

/**
 * dir 0: CCW irt encoder
 * dir 1: CW irt encoder
 * else, stop
 * Pins must be driven low before high to prevent shoot through.
 */
void runMotor(int dir) {
  if (dir == 0) {
    digitalWrite(motor_IN2, LOW);
    digitalWrite(motor_IN1, HIGH);
  } else if (dir == 1) {
    digitalWrite(motor_IN1, LOW);
    digitalWrite(motor_IN2, HIGH);
  } else {
    digitalWrite(motor_IN1, LOW);
    digitalWrite(motor_IN2, LOW);
  }
}

void servoWrite(int angle) {
  int rotations = angle / 360;
}

void driveMotorPID(double PIDOutput) {
  int duty = (int) abs(PIDOutput);
  duty = constrain(duty, 0, BIT_12_LIMIT);

  if (PIDOutput <= 0) {
    ledcWrite(pwmChannel2, 0);
    ledcWrite(pwmChannel1, duty);
  } else {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, duty);
  }
}

// Computes the smallest angular distance from the input angle to the set angle
// Positive if set has a "higher" value than input
// Output Range: -180 to 180
float computeAngleError(float targetAngle, float inputAngle) {
  float error = targetAngle - inputAngle; // Must be between (-360 to 360)
  // Make sure it's between -360 to 360
  error = fmod(error, 360);

  if (error > 180) {
      error -= 360;
  } else if (error < -180) {
      error += 360;
  }

  return error;
}