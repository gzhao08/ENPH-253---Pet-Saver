#include <Arduino.h>
#include "SteeringManager.h"  
#include "esp_task_wdt.h"
#include "WifiHelper.h"
#include "drivetrain.h"
#include "../GlobalConstants.h"
#include "SharedState.h"

void driveTrain(void *parameter) {
    esp_task_wdt_delete(NULL);
    //----------------------------SETUP----------------------------//
    DCMotor left = DCMotor(LEFT_MOTOR_PIN_A,LEFT_MOTOR_PIN_B,LEFT_MOTOR_PWM_CHANNEL_A,LEFT_MOTOR_PWM_CHANNEL_B,12); 
    DCMotor right = DCMotor(RIGHT_MOTOR_PIN_A,RIGHT_MOTOR_PIN_B,RIGHT_MOTOR_PWM_CHANNEL_A,RIGHT_MOTOR_PWM_CHANNEL_B,12);    
    SteeringManager steer(&left,&right);

    double kp = 160.0;
    double kd = 0;
    int baseSpeed = 1050;

    left.begin();
    right.begin();

    steer.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN); // put IR pins here -> left to right

    Serial.begin(115200);
    steer.setPID(kp,kd);
    delay(2000);

    startRead = true; // start reading the lidar data

    //----------------------------LOOP----------------------------//
    while (true) {
     
        while(!drive) {
            //Serial.println("Waiting for drive to be true");
            vTaskDelay(10);
        }
        steer.forward(baseSpeed);
    }
    
    
}