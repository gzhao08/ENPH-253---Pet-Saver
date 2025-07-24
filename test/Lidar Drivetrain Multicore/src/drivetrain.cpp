#include <Arduino.h>
#include "../lib/SteeringManager/SteeringManager.h"  
#include "../lib/WifiHelper/WifiHelper.h"
#include "drivetrain.h"
#include "../lib/GlobalConstants.h"

void driveTrain(void *parameter) {

    //----------------------------SETUP----------------------------//
    DCMotor left = DCMotor(22,19,1,2,15); 
    DCMotor right = DCMotor(20,21,3,4,15);    

    SteeringManager steer(&left,&right);

    double kp = 0;
    double kd = 0;
    int baseSpeed = 800;

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
    delay(2000);

    startRead = true; // start reading the lidar data


    //----------------------------LOOP----------------------------//
    // Line Following

    while (true) {
        // Before doorway: SECTION 1
        steer.lineFollow(SECTION_0_SPEED);
        // After doorway -- sense pet and turn
        delay(3000);
        steer.lineFollow(SECTION_1_SPEED);
        // After turn -- ramp
        steer.lineFollow(SECTION_2_SPEED);
        delay(1000);
        // After ramp -- lower speed
        steer.lineFollow(700);
    }
    
}