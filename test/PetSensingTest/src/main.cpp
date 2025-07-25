#include <Wire.h>
#include <Adafruit_LIS3MDL.h>
#include <GlobalConstants.h>
#define NUM_TRIALS 50
#define THRESH 50

#include "managers/WireManager.h"
#include "managers/DelayManager.h"
#include "motors/Servo.h"
#include "motors/ContinuousServo.h"
#include "claw/ClawGrabber.h"
#include "claw/ClawArm.h"
#include "claw/ClawVerticalStage.h"
#include "sensors/Microswitch.h"

float last_reading = 0.0;

Adafruit_LIS3MDL lis3mdl;

int muxPin = 8;
WireManager wireManager(muxPin); 

int armMotorPin1 = 2;
int armMotorPin2 = 12;
int armPwmChannel1 = 2; //motor pin 1 goes to B
int armPwmChannel2 = 3; 
int armMuxLine = 1; 
bool armEncoderOnTerminalSide = true; //false for arm, 
int armSwitchPin = 37; 
bool armNormallyOpen = true; 

DelayManager positionDelayManager(4000);
//muxLine: 1 is 1, 0 is 2, -1 is not muxing
ClawArm arm(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
  armSwitchPin, armNormallyOpen); 

int verticalStageMotorPin1 = 33;
int verticalStageMotorPin2 = 32;
int verticalStagePwmChannel1 = 4; //motor pin 1 goes to B
int verticalStagePwmChannel2 = 5; 
int verticalStageMuxLine = 0; 
bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
int verticalStageSwitchPin = 38; 
bool verticalStageNormallyOpen = true; 

ClawVerticalStage verticalStage(verticalStageMotorPin1, verticalStageMotorPin2, verticalStagePwmChannel1, verticalStagePwmChannel2, verticalStageMuxLine, verticalStageEncoderOnTerminalSide,
  verticalStageSwitchPin, verticalStageNormallyOpen);

DelayManager printDelayManager(2000);
DelayManager moveDelayManager(1000);

float bestPosition = 0;
float bestReading = 0;


bool directionForward = true;

void setup() {
  Serial.begin(115200);

  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  // 3. Begin servo
  arm.begin(&wireManager); 
  verticalStage.begin(&wireManager);
  positionDelayManager.reset();
  printDelayManager.reset();
  
  delay(1000);
  Serial.println("LIS3MDL test");

  //Wire.begin(25, 26);  // defaults to GPIO21 (SDA), GPIO22 (SCL) on ESP32

  if (!lis3mdl.begin_I2C(LIS3MDL_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) delay(10);
  }

  Serial.println("LIS3MDL Found!");
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.println("ready set go");
    arm.homingSequence();

  verticalStage.homingSequence();
  verticalStage.setPosition(200);
  Serial.println("HOME!");
  arm.setPosition(100);

  verticalStage.motorVertical.logPIDOutput = true;

  // verticalStage.setPIDTuningMode(true);
  // verticalStage.setPIDTuningPins(34,35); // P and D pins for tuning
}

void loop() {
  // MAGNETOMETER STUFF
  while (millis() < 7000) {
    verticalStage.loop();
    Serial.println("Waiting for vertical stage to reach target position...");
  }

  float x_tot = 0;
  float y_tot = 0;
  float z_tot = 0;
  sensors_event_t event;
  for (int i = 0; i < NUM_TRIALS; i++) {
    lis3mdl.getEvent(&event);
    x_tot += event.magnetic.x;
    y_tot += event.magnetic.y;
    z_tot += event.magnetic.z;
  }
  x_tot /= NUM_TRIALS;
  y_tot /= NUM_TRIALS;
  z_tot /= NUM_TRIALS;
  float magnitude = sqrt(x_tot*x_tot+y_tot*y_tot+z_tot*z_tot);

  if (printDelayManager.checkAndReset()) {
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print(" uT, ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print(" uT, ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.println(" uT");
  }

  //Serial.printf("%f,%f,%f,%f\n", x_tot, y_tot, z_tot,magnitude);
  // if (abs(last_reading - z_tot) > THRESH) {
  //   Serial.println("unstable Reading, no field detected");
  // }
  // else {
  //       Serial.printf("%f\n",z_tot);

  // }
  

  // CLAW STUFF
  if (directionForward) {
    Serial.println("Moving forward");
    if (moveDelayManager.checkAndReset()) {
      Serial.println("Moving arm");

      if (magnitude > bestReading) {
        bestPosition = arm.getPosition();
        bestReading = magnitude;
        Serial.println("Better position ");
      } else {
        Serial.println("Worse position ");
      }

      // Move the arm to a new position
      int increment = 20;
      if (!directionForward) {
        increment = -increment;
      }
      float newPosition = arm.getPosition() + increment;
      if (abs(arm.MAX_POSITION - newPosition) < 30 || abs(newPosition - arm.MIN_POSITION) < 10) {
        Serial.println("CHANGED DIRECTION");
        directionForward = !directionForward; // Cap at max height
      }
      arm.setPosition(newPosition);
      Serial.print("Arm Position: ");
      Serial.println(newPosition);
    }
  } else {
    arm.setPosition(bestPosition-80);
    verticalStage.setPosition(verticalStage.getPosition() - 150);
  }

  arm.loop();
  verticalStage.loop();
}
