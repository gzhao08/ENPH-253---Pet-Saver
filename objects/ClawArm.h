#pragma once

#include "ContinuousServo.h"
#include "Microswitch.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawArm {
    private:
        ContinuousServo* motor;
        Microswitch* mswitch;

        //position in degrees
        float position = -1;
        
    public: 
        int MIN_POSITION = 0;
        int MAX_POSITION;

        ClawArm();

        // Call in setup
        void begin(ContinuousServo* motor, Microswitch* mswitch);

        float getPosition();
        void setPosition(float distance);
        void testSequence();
    
};