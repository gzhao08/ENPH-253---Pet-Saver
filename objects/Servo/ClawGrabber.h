#pragma once

#include "Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawGrabber {
    private:
        Servo* motor;
        
    public: 
        float OPEN = 11.9;     
        float CLOSE = 9.4;
        float PET_CLOSE = 9.4;
        float PARALLEL = 10.5;

        ClawGrabber();

        // Call in setup
        void begin(Servo* motor);

        float getPositionDegrees();
        float getPositionDutyCycle();
        void setPositionDegrees(float angle);
        void setPositionDutyCycle(float cycle);
        void setParallel();
        void testSequence();
    
};