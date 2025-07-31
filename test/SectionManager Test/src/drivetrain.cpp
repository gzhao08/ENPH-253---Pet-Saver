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

    double kp = 160.0;
    double kd = 0;
    int baseSpeed = 1050;

    steer.begin(); // put IR pins here -> left to right

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
        steer.lineFollow(1300);
        // steer.backward(1000);
        delay(1000);
    }
    
    
}