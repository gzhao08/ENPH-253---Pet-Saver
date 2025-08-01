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
    SteeringManager steer;

    // double kp = 160.0;
    // double kd = 0;
    // int baseSpeed = 1050;

    steer.begin(); // put IR pins here -> left to right

    Serial.begin(115200);
    steer.setPID(KP_DEFAULT,0);
    delay(2000);

    startRead = true; // start reading the lidar data

    int speed = 600;

    //----------------------------LOOP----------------------------//
    while (true) {
     
        while(!drive) {
            //Serial.println("Waiting for drive to be true");
            vTaskDelay(10);
        }
        steer.lineFollow(1300); // up to doorway
        steer.lineFollow(700);  // up to pet 1
        steer.quickStop();      // stop at pet 1
        steer.lineFollow(800); //up to beginning of ramp
        steer.quickStop();// stop at beginning of ramp
        steer.lineFollow(1500);// up to end of ramp
        steer.quickStop();// stop at end of ramp
        delay(10000);


        // steer.leftMotor.drivePWM(speed%1500);
        // speed += 100;
        // delay(1000);
        // steer.leftMotor.stop();
        // delay(500);
    }
    
    
}