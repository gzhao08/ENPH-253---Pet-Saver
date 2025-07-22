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

  steer.begin(38,37,39,36); // put IR pins here -> left to right

  Serial.begin(115200);
}

void loop() {
  steer.array.takeReading(true);
  steer.array.getError();
  //steer.array.showState();
  steer.array.update();
  delay(50);
}