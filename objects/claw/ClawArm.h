#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawArm {
    private:
        ContinuousServo motorArm;
        Microswitch mswitchArm;

        //position in degrees
        float position = -1;
        
    public: 
        int MIN = 0;
        int MAX = 100;

        ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen);

        // Call in setup
        void begin(WireManager* wireManager);

        void home();
        float getPosition();
        void setPosition(float distance);
        void testSequence();
        void loop();
    
};