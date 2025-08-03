#include <Arduino.h>
#include "SteeringManager.h"  
#include <WiFi.h>
#include "WifiHelper.h" 
#include "../GlobalConstants.h"
    
DCMotor left = DCMotor(LEFT_MOTOR_PIN_A,LEFT_MOTOR_PIN_B,LEFT_MOTOR_PWM_CHANNEL_A,LEFT_MOTOR_PWM_CHANNEL_B,12); 
DCMotor right = DCMotor(RIGHT_MOTOR_PIN_A,RIGHT_MOTOR_PIN_B,RIGHT_MOTOR_PWM_CHANNEL_A,RIGHT_MOTOR_PWM_CHANNEL_B,12);    

SteeringManager steer;

int baseSpeed = 800;



void setup() {
  left.begin();
  right.begin();

  steer.begin(); // put IR pins here -> left to right vp: 36 vn: 39

  Serial.begin(115200);
  Serial.println("Serial Started");

  // WifiHelper wifi = WifiHelper();
  // wifi.begin();
  // Serial.println("Wifi Server Started");
  // wifi.startTune(&kp, &kd, &baseSpeed); // try kp = 210, baseSpeed = 875
  // baseSpeed = constrain(baseSpeed,0,1500);
  // kp = 150;
  // baseSpeed = 850;
  steer.setPID(KP_DEFAULT,KD_DEFAULT); 
  delay(1000);
  
}

void loop() {
  currentSpeed = 1400;
  robotState = RobotState::LINE_FOLLOW;
  steer.lineFollow();


}