#include <Arduino.h>
#include "SteeringManager.h"  
#include "esp_task_wdt.h"
#include "DriveTrainTest.h"
#include "sharedState"

void driveTrain(void *parameter) {
  esp_task_wdt_delete(NULL);
  DCMotor left = DCMotor(LEFT_MOTOR_PIN_A,LEFT_MOTOR_PIN_B,LEFT_MOTOR_PWM_CHANNEL_A,LEFT_MOTOR_PWM_CHANNEL_B,12); 
  DCMotor right = DCMotor(RIGHT_MOTOR_PIN_A,RIGHT_MOTOR_PIN_B,RIGHT_MOTOR_PWM_CHANNEL_A,RIGHT_MOTOR_PWM_CHANNEL_B,12);    
  SteeringManager steer(&left,&right);

  double kp = 160.0;
  double kd = 0;
  int baseSpeed = 1050;

  left.begin();
  right.begin();

  steer.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN); // put IR pins here -> left to right
  steer.setPID(kp,kd);
  delay(2000);

  startRead = true; // start reading the lidar data

  while (true) {
    steer.lineFollow(700);

  }

}
