#include <Arduino.h>
#include "SteeringManager.h"  
#include <WiFi.h>
#include "WifiHelper.h" 
#include "../GlobalConstants.h"

    
DCMotor left = DCMotor(LEFT_MOTOR_PIN_A,LEFT_MOTOR_PIN_B,LEFT_MOTOR_PWM_CHANNEL_A,LEFT_MOTOR_PWM_CHANNEL_B,12); 
DCMotor right = DCMotor(RIGHT_MOTOR_PIN_A,RIGHT_MOTOR_PIN_B,RIGHT_MOTOR_PWM_CHANNEL_A,RIGHT_MOTOR_PWM_CHANNEL_B,12);    

SteeringManager steer(&left,&right);

double kp = 0;
double kd = 0;
int baseSpeed = 800;


void setup() {
  left.begin();
  right.begin();

  steer.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN); // put IR pins here -> left to right vp: 36 vn: 39

  Serial.begin(115200);

  WifiHelper wifi = WifiHelper();
  wifi.begin();
  Serial.println("Wifi Server Started");
  wifi.startTune(&kp, &kd, &baseSpeed); // try kp = 210, baseSpeed = 875
  baseSpeed = constrain(baseSpeed,0,1500);
  steer.setPID(kp,kd); 
  delay(2000);
  
}

void loop() {
  
  // Line Following
  steer.lineFollow(baseSpeed);

  // steer.array.takeReading(false);
  // steer.array.getError();
  // steer.array.showState();
  // Serial.print("\n");
  // steer.array.update();

  // Serial.println("begin");
  // left.drivePWM(800);
  // delay(2000);
  // right.drivePWM(800);
  // delay(2000);


}