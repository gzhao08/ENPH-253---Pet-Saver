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
#include "claw/ClawBase.h"
#include "sensors/Microswitch.h"
#include "sensors/Microswitch.h"

float last_reading = 0.0;

Adafruit_LIS3MDL lis3mdl;

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

Microswitch resetSwitch(verticalStageSwitchPin, true);

int baseMotorPin1 = 14;
int baseMotorPin2 = 27;
int basePwmChannel1 = 6; //motor pin 1 goes to B
int basePwmChannel2 = 7; 
int baseMuxLine = -1; 
bool baseEncoderOnTerminalSide = false; //false for arm, 
int baseSwitchPin = 9; 
bool baseNormallyOpen = true; 

ClawBase base(baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, baseSwitchPin, baseNormallyOpen);


DelayManager printDelayManager(2000);
DelayManager moveDelayManager(200);

float bestPosition = 0;
float bestReading = 0;


bool directionForward = true;

float getMagnetReadingMagSq();
void sensePet();
void calibrateMagnet();

void setup() {
  Serial.begin(115200);

  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(7, 5);
  Wire1.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);
  // 2. Begin wire manager
  wireManager.begin(&Wire);
  wireManager2.begin(&Wire1);
  // 3. Begin servo
  arm.begin(&wireManager); 
  verticalStage.begin(&wireManager);
  base.begin(&wireManager2);

  positionDelayManager.reset();
  printDelayManager.reset();
  resetSwitch.begin();
  
  delay(1000);
  Serial.println("LIS3MDL test");

  if (!lis3mdl.begin_I2C(LIS3MDL_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) delay(10);
  }

  Serial.println("LIS3MDL Found!");
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);


  Serial.println("Starting homing sequence");
  arm.homingSequence();
  verticalStage.homingSequence();
  verticalStage.setPosition(80);
  while (!verticalStage.reachedTarget()) {
    verticalStage.loop();
  }
  base.homingSequence();

  calibrateMagnet();


    // base.setAsHome();


  // verticalStage.homingSequence();
  // verticalStage.setPosition(100);
  Serial.println("HOME!");
  // arm.setPosition(100);
  // base.setPosition(60);

  verticalStage.continuousServo.logPIDOutput = true;

  sensePet();
}

void loop() {
  if (resetSwitch.isPressed()) {
    sensePet();
  }
  // // MAGNETOMETER STUFF
  // while (millis() < 7000) {
  //   verticalStage.loop();
  //   base.loop();
  //   Serial.println("Waiting for vertical stage to reach target position...");
  // }

  // float x_tot = 0;
  // float y_tot = 0;
  // float z_tot = 0;
  // sensors_event_t event;
  // for (int i = 0; i < NUM_TRIALS; i++) {
  //   lis3mdl.getEvent(&event);
  //   x_tot += event.magnetic.x;
  //   y_tot += event.magnetic.y;
  //   z_tot += event.magnetic.z;
  // }
  // x_tot /= NUM_TRIALS;
  // y_tot /= NUM_TRIALS;
  // z_tot /= NUM_TRIALS;
  // float magnitude = sqrt(x_tot*x_tot+y_tot*y_tot+z_tot*z_tot);

  // if (printDelayManager.checkAndReset()) {
  //   Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print(" uT, ");
  //   Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print(" uT, ");
  //   Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.println(" uT");
  // }

  // //Serial.printf("%f,%f,%f,%f\n", x_tot, y_tot, z_tot,magnitude);
  // // if (abs(last_reading - z_tot) > THRESH) {
  // //   Serial.println("unstable Reading, no field detected");
  // // }
  // // else {
  // //       Serial.printf("%f\n",z_tot);

  // // }
  

  // // CLAW STUFF
  // if (directionForward) {
  //   Serial.println("Moving forward");
  //   if (moveDelayManager.checkAndReset()) {
  //     Serial.println("Moving arm");

  //     if (magnitude > bestReading) {
  //       bestPosition = arm.getPosition();
  //       bestReading = magnitude;
  //       Serial.println("Better position ");
  //     } else {
  //       Serial.println("Worse position ");
  //     }

  //     // Move the arm to a new position
  //     int increment = 10;
  //     if (!directionForward) {
  //       increment = -increment;
  //     }
  //     float newPosition = arm.getPosition() + increment;
  //     if (abs(arm.MAX_POSITION - newPosition) < 10 || abs(newPosition - arm.MIN_POSITION) < 5) {
  //       Serial.println("CHANGED DIRECTION");
  //       directionForward = !directionForward; // Cap at max height
  //     }
  //     arm.setPosition(newPosition);
  //     Serial.print("Arm Position: ");
  //     Serial.println(newPosition);
  //   }
  // } else {
  //   Serial.println("DIRECTION NOT FORWARD");
  //   arm.setPosition(bestPosition-80);
  //   verticalStage.setPosition(verticalStage.getPosition() - 150);
  // }

  // arm.loop();
  // verticalStage.loop();
}

float getMagnetReadingMagSq() {
  // Remember to benchmark
  float sampleSize = 10;
  float x_tot = 0;
  float y_tot = 0;
  float z_tot = 0;
  sensors_event_t event;

  float HARD_IRON_OFFSET_X = -20.3;
  float HARD_IRON_OFFSET_Y = 87.5;
  float HARD_IRON_OFFSET_Z = 19.3;

  for (int i = 0; i < sampleSize; i++) {
    lis3mdl.getEvent(&event);
    x_tot += event.magnetic.x - HARD_IRON_OFFSET_X;
    y_tot += event.magnetic.y - HARD_IRON_OFFSET_Y;
    z_tot += event.magnetic.z - HARD_IRON_OFFSET_Z;
  }
  x_tot /= sampleSize;
  y_tot /= sampleSize;
  z_tot /= sampleSize;

  // Serial.printf("X Reading: %.2f\n", x_tot);
  // Serial.printf("Y Reading: %.2f\n", y_tot);
  // Serial.printf("Z Reading: %.2f\n", z_tot);
  return x_tot*x_tot + y_tot*y_tot + z_tot*z_tot;
}

void sensePet() {
  int samples = 0;
  Serial.println("Sensing pet start");
  // Params:
  int baseInit = -30;
  int verticalInit = 80;
  int armInit = 80;

  const int MAGNETIC_THRESHOLD = 1000;

  // verticalStage.setPosition(verticalInit);
  arm.setPosition(armInit);
  base.setPosition(baseInit);

  while (!arm.reachedTarget() || !base.reachedTarget()) {
    Serial.println("Going to initial");
    // Serial.println(getMagnetReadingMagSq());
    arm.loop();
    // verticalStage.loop();
    base.loop();
  }
  Serial.println("Initial position reached");

  // Try find best angle

  int sweepAngle = 30; // one side from initial position (so movement 2x sweepangle)
  int sweepAngle2 = 20;
  int sweepAngle3 = 10;

  int maxMagnetReading = 0;
  int maxMagnetReadingPos = 0;

  // First sweep
  base.setPosition(baseInit + sweepAngle);
  while (!base.reachedTarget()) {
    Serial.println("Moving correct base to position");
    base.loop();
    arm.loop();
    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);
    if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }
  base.setPosition(baseInit - sweepAngle);
  while (!base.reachedTarget()) {
    Serial.println("Moving correct base to position");
    base.loop();
    arm.loop();
    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);
    if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }

  // Second sweep
  base.setPosition(maxMagnetReadingPos+sweepAngle2);
  while (!base.reachedTarget()) {
    Serial.println("Moving to other base position");
    base.loop();
    arm.loop();

    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);

    if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }
  base.setPosition(maxMagnetReadingPos-sweepAngle2);
  while (!base.reachedTarget()) {
    Serial.println("Moving to other base position");
    base.loop();
    arm.loop();

    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);

    if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }

  // Third sweep
  base.setPosition(maxMagnetReadingPos+sweepAngle3);
  while (!base.reachedTarget()) {
    Serial.println("Moving correct base to position");
    base.loop();
    arm.loop();
    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);
    if (maxMagnetReading < currentReading) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }
  base.setPosition(maxMagnetReadingPos-sweepAngle3);
  while (!base.reachedTarget()) {
    Serial.println("Moving correct base to position");
    base.loop();
    arm.loop();
    float currentReading = getMagnetReadingMagSq();
    Serial.println(currentReading);
    if (maxMagnetReading < currentReading) {
      maxMagnetReading = currentReading;
      maxMagnetReadingPos = base.getPosition();
    }
    samples += 1;
  }

  base.setPosition(maxMagnetReadingPos);
  while (!base.reachedTarget()) {
    Serial.println("moving to max reading position");
    base.loop();
    arm.loop();

  }

  Serial.printf("Max magnet reading position: %d\n", maxMagnetReadingPos);
  Serial.printf("Done, samples: %d\n", samples);

  // // Try to find best position
  // int armExtension = 200;
  // float maxMagnetReadingArm = 0;
  // float maxMagnetReadingArmPos = 0;
  // arm.setPosition(armInit + armExtension);
  // while (!arm.reachedTarget()) {
  //   base.loop();
  //   arm.loop();
  //   float currentReading = getMagnetReadingMagSq();
  //   if (maxMagnetReadingArm < currentReading) {
  //     maxMagnetReadingArm = currentReading;
  //     maxMagnetReadingArmPos = arm.getPosition();
  //   }
  // }

  // arm.setPosition(maxMagnetReadingArmPos);
  // while (!arm.reachedTarget()) {
  //   arm.loop();
  //   base.loop();
  // }

}



void calibrateMagnet() {
  float min_x, max_x, mid_x;
  float min_y, max_y, mid_y;
  float min_z, max_z, mid_z;
  
  sensors_event_t event;
  lis3mdl.getEvent(&event);

  min_x = max_x = event.magnetic.x;
  min_y = max_y = event.magnetic.y;
  min_z = max_z = event.magnetic.z;
  delay(10);

  while (true) {
    lis3mdl.getEvent(&event);
    float x = event.magnetic.x;
    float y = event.magnetic.y;
    float z = event.magnetic.z;
    
    Serial.print("Mag: (");
    Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", ");
    Serial.print(z); Serial.print(")");

    min_x = min(min_x, x);
    min_y = min(min_y, y);
    min_z = min(min_z, z);

    max_x = max(max_x, x);
    max_y = max(max_y, y);
    max_z = max(max_z, z);

    mid_x = (max_x + min_x) / 2;
    mid_y = (max_y + min_y) / 2;
    mid_z = (max_z + min_z) / 2;
    Serial.print(" Hard offset: (");
    Serial.print(mid_x); Serial.print(", ");
    Serial.print(mid_y); Serial.print(", ");
    Serial.print(mid_z); Serial.print(")");  

    Serial.print(" Field: (");
    Serial.print((max_x - min_x)/2); Serial.print(", ");
    Serial.print((max_y - min_y)/2); Serial.print(", ");
    Serial.print((max_z - min_z)/2); Serial.println(")");    
    delay(10); 
  }
}

void test() {
  //   last_x = x_tot/sampleSize;
  // last_y = y_tot/sampleSize;
  // last_z = z_tot/sampleSize;
  

  // float lastReading = getMagnetReadingMagSq();
  // int lastPosition = base.getPosition();
  // int bestPosition = lastPosition;
  // float bestReading = lastReading;
  // while (true) {
  //   base.loop();
  //   float currentReading = getMagnetReadingMagSq();
  //   int currentPosition = base.getPosition();
  //   // Serial.printf("Squared reading: %.2f\n", getMagnetReadingMagSq());
  //   Serial.printf("Change squared reading: %.2f\n", getMagnetReadingMagSq());

  //   float delta = currentReading - lastReading;
  //   Serial.printf("Change in reading: %.2f\n",delta);
  //   Serial.printf("Current position: %d\n", currentPosition);
  //   Serial.printf("Best position: %d\n", bestPosition);
  //   Serial.printf("Best reading: %.2f\n", bestReading);
  //   Serial.println("---");

  //   if (abs(delta) >= abs(bestReading)) {
  //     bestPosition = currentPosition;
  //     bestReading = currentReading;
  //   }

  //   lastReading = currentReading;
  //   delay(500);
  // }
}