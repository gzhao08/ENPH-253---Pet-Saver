#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/Servo.h"

class ClawGrabber {
    public: 
        MyServo motor;

        // Calibrated with duty cycle
        float OPEN_DUTY = 11;     
        float CLOSE_DUTY = 8;
        float PET_CLOSE_DUTY = 8;
        float PARALLEL_DUTY = 11;

        // Calibrated with angular position
        const int OPEN_DEG = 0;
        const int PARALLEL_DEG = 50;
        const int CLOSE_DEG = 100;

        ClawGrabber(int motorPin, int pwmChannel);

        // Call in setup
        void begin();

        float getPositionDegrees();
        float getPositionDutyCycle();
        void setPositionDegrees(float angle);
        void setPositionDutyCycle(float cycle);
        void setParallel();
        void testSequence();

        void close();
        void open();
        void parallel();
    
};