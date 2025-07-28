#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/Servo.h"

class ClawGrabber {
    private:
        MyServo motor;
        
    public: 
        float OPEN = 11;     
        float CLOSE = 8;
        float PET_CLOSE = 8;
        float PARALLEL = 11;

        float OPEN_DEG = 0;
        float CLOSE_DEG = 100;

        ClawGrabber(int motorPin, int pwmChannel);

        // Call in setup
        void begin();

        float getPositionDegrees();
        float getPositionDutyCycle();
        void setPositionDegrees(float angle);
        void setPositionDutyCycle(float cycle);
        void setParallel();
        void testSequence();
    
};