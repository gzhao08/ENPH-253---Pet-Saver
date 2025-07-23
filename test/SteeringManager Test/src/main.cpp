#include <Arduino.h>
#include "SteeringManager.h"  
#include <WiFi.h>
#include "WifiHelper.h" 
    
DCMotor left = DCMotor(22,19,1,2,15); 
DCMotor right = DCMotor(20,21,3,4,15);    

SteeringManager steer(&left,&right);

double kp = 0;
double kd = 0;
int baseSpeed = 800;


void setup() {
  left.begin();
  right.begin();

  steer.begin(39,36,38,37); // put IR pins here -> left to right

  Serial.begin(115200);

  WifiHelper wifi = WifiHelper();
  wifi.begin();
  Serial.println("Wifi Server Started");
  wifi.startTune(&kp, &kd, &baseSpeed);
  baseSpeed = constrain(baseSpeed,0,1000);
  steer.setPID(kp,kd);

  
}

void loop() {
  // steer.array.takeReading(true);
  // steer.array.getError();
  // steer.array.update();
  // delay(50);

  //steer.lineFollow(baseSpeed);

  // left.drivePWM(1200);
  // right.drivePWM(1200);
  // delay(5000);
  // left.drivePWM(400);
  // right.drivePWM(400);
  // delay(5000);
}