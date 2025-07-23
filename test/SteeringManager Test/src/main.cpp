#include <Arduino.h>
#include "SteeringManager.h"  
// #include <WiFi.h>
#include "WifiHelper.h" 
    
DCMotor left = DCMotor(19,22,1,2,15); 
DCMotor right = DCMotor(21,20,3,4,15);    

SteeringManager steer(&left,&right);

double kp = 0;
double kd = 0;


void setup() {
  left.begin();
  right.begin();

  steer.begin(38,37,39,36); // put IR pins here -> left to right

  Serial.begin(115200);

  // WifiHelper wifi = WifiHelper();
  // wifi.begin();
  // Serial.println("Wifi Server Started");
  // wifi.startTune(&kp, &kd);
}

void loop() {
  // steer.forward(2000);
  // delay(2000);
  // steer.backward(2000);
  // delay(2000);

  steer.array.takeReading(true);
  steer.array.getError();
  steer.array.update();
  delay(50);

  //steer.lineFollow(800);
}