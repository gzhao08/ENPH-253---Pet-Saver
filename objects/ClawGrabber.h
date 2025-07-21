#pragma once

#include "Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawGrabber {
    private:
        Servo* motor;
        
    public: 
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