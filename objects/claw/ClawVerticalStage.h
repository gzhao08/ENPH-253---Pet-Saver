#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawVerticalStage {
    private:
        ContinuousServo motorVertical;
        Microswitch mswitchVertical;

        float height = -1; //height in mm
        int pitch = 2; // pitch in mm

    public: 

        int MIN_HEIGHT = 0;
        int MAX_HEIGHT;
        int RESET_HEIGHT; 
        int LOW_PET; 
        int HIGH_PET; 
        int LOW_SENSE; 
        int HIGH_SENSE; 

        ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen);

        // Call in setup
        void begin(WireManager* wireManager);

        float getPosition();
        void setPosition(float angle);
        void testSequence();
        void home();
    
};