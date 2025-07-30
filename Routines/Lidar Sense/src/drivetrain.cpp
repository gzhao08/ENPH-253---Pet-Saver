#include <Arduino.h>
#include "SteeringManager.h"  
#include "esp_task_wdt.h"
#include "WifiHelper.h"
#include "drivetrain.h"
#include "../GlobalConstants.h"

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

    // WifiHelper wifi = WifiHelper();
    // wifi.begin();
    // Serial.println("Wifi Server Started");
    // wifi.startTune(&kp, &kd, &baseSpeed);
    // wifi.end();
    // baseSpeed = constrain(baseSpeed,0,1000);
    // try setting base speed to be 875 and kp to be 210
    steer.setPID(kp,kd);
    delay(2000);

    startRead = true; // start reading the lidar data

    

    //----------------------------LOOP----------------------------//
    // Line Following

    // // Before doorway: SECTION 1
    // steer.lineFollow(SECTION_0_SPEED);
    // // After doorway -- sense pet and turn
    // delay(3000);
    // steer.lineFollow(SECTION_1_SPEED);
    // // After turn -- ramp
    // delay(1000);
    // steer.lineFollow(SECTION_2_SPEED);
    // delay(1000);
    // // After ramp -- lower speed
    // steer.lineFollow(700);
    while (true) {
        // Before doorway: SECTION 1
        while(!drive) {
            Serial.println("Waiting for drive to be true");
            vTaskDelay(10);
        }
        //Serial.println(drive);
        Serial.printf("drive address (drivetrain): %p\n", (void*)&drive);


        steer.lineFollow(900);

        while(!drive) {
             vTaskDelay(10);
        }
        // After doorway, before first pet
        
        steer.lineFollow(700);
        while(!drive) {
             vTaskDelay(10);
        }
        // after first pet, before ramp
        steer.lineFollow(700);
        while(!drive) {
             vTaskDelay(10);
        }
        // after finding ramp
        steer.lineFollow(1100);
        while(!drive) {
             vTaskDelay(10);
        }



        // steer.lineFollow(800);
        // // After doorway -- sense pet and turn
        // delay(3000);
        // steer.lineFollow(800);
        // // After turn -- ramp
        // delay(1000);
        // steer.lineFollow(800);
        // delay(1000);
        // // After ramp -- lower speed
        // steer.lineFollow(700);
    }
    
    
}