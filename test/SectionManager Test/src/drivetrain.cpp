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
    steer.setPID(KP_DEFAULT,KD_DEFAULT);
    delay(2000);

    startRead = true; // start reading the lidar data

    int speed = 600;

    //----------------------------LOOP----------------------------//
    while (true) {
     
        switch (robotState) {
            case RobotState::LINE_FOLLOW: {
                Serial.println("drivetrain.cpp: LINE_FOLLOW");
                steer.lineFollow();
                break;
            }
            case RobotState::STOPPED: {
                //Serial.println("drivetrain.cpp: LINE_FOLLOW");
                steer.quickStop();
                while(robotState == STOPPED) {
                    delay(10);
                }
                break;
            }
            case RobotState::TURN_CW:
                steer.turnAround(currentSpeed,true);
                break;
            case RobotState::TURN_CCW:
                steer.turnAround(currentSpeed,false);
                break;
            case RobotState::FORWARD: {
                steer.forward(currentSpeed);
                break;
            }
            case RobotState::TURN_CW_BACK: {
                steer.turnBackwards(currentSpeed);
                break;
            }
            
        }
        delay(10);
    }
    
    
}