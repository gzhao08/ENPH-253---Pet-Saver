#include <Arduino.h> 
#include "IRArray.h"
#include "DCMotor.h" 
#include "SteeringManager.h"


DCMotor left = DCMotor(25,26,1,2,15); 
DCMotor right = DCMotor(32,33,3,4,15);    

SteeringManager steer(&left,&right);

void setup() {
  left.begin();
  right.begin();

  steer.begin();

  Serial.begin(115200);
}

void loop() {
  steer.array.showState();
  delay(50);
}