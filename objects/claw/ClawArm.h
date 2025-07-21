#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawArm {
    private:
        ContinuousServo motor;
        Microswitch mswitch;

        //position in degrees
        float position = -1;
        
    public: 
        int MIN = 0;
        int MAX;

        ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin);

        // Call in setup
        void begin();

        void home();
        float getPosition();
        void setPosition(float distance);
        void testSequence();
    
};