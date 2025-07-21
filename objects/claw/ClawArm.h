#pragma once


#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Our library
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawArm {
    private:
        ContinuousServo* motor;
        Microswitch* mswitch;

        //position in degrees
        float position = -1;
        
    public: 
        int MIN = 0;
        int MAX;

        ClawArm();

        // Call in setup
        void begin(ContinuousServo* motor, Microswitch* mswitch);

        float getPosition();
        void setPosition(float distance);
        void testSequence();
    
};