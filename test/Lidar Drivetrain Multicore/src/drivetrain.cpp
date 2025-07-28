#include <Arduino.h>
#include "../lib/SteeringManager/SteeringManager.h"  
#include "../lib/WifiHelper/WifiHelper.h"
#include "drivetrain.h"
#include "../lib/GlobalConstants.h"

void driveTrain(void *parameter) {

    //----------------------------SETUP----------------------------//
    DCMotor left = DCMotor(22,19,1,2,12); 
    DCMotor right = DCMotor(20,21,3,4,12);    

    SteeringManager steer(&left,&right);

    double kp = 170.0;
    double kd = 0;
    int baseSpeed = 800;

    left.begin();
    right.begin();

    steer.begin(35,34,36,39); // put IR pins here -> left to right

    Serial.begin(115200);

    WifiHelper wifi = WifiHelper();
    wifi.begin();
    Serial.println("Wifi Server Started");
    wifi.startTune(&kp, &kd, &baseSpeed);
    wifi.end();
    baseSpeed = constrain(baseSpeed,0,1000);
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
        steer.lineFollow(600);
        delay(1000);
        // steer.lineFollow(600);
        // delay(1000);
        // steer.lineFollow(1000);
        // delay(1000);
    }
    
    
}