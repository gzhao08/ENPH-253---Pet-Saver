#pragma once


#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawBase {
    private:
        ContinuousServo motor;
        Microswitch mswitch;

        //position in degrees
        float position = -1;
        float MIN_POSITION = -120;
        float MAX_POSITION = 120;

    public: 

        ClawBase(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin);

        // Call in setup
        void begin();

        void home();
        float getPosition();
        void setPosition(float angle);
        void testSequence();
    
};