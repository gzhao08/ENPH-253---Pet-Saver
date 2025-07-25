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
        float OPEN = 11.9;     
        float CLOSE = 9.4;
        float PET_CLOSE = 9.4;
        float PARALLEL = 11;

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