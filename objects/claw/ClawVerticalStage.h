#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawVerticalStage {
    private:
        Microswitch mswitchVertical;

        const float ANGLE_TO_MM_CONVERSION = -0.022; // Calibrating from 2mm pitch * 4 starts / 360 degrees
        const float MM_TO_ANGLE_CONVERSION = 1 / ANGLE_TO_MM_CONVERSION;

        const int MIN_HEIGHT = 0;
        const int MAX_HEIGHT = 100;
        const int IDLE_HEIGHT = 50;

        const int LOW_PET_HEIGHT = 10;
        const int HIGH_PET_HEIGHT = 90;
        const int LOW_PET_SENSING_HEIGHT = 20;
        const int HIGH_PET_SENSING_HEIGHT = 80;
        const int PID_P = 55;
        const int PID_D = 0.7;

    public: 
        ContinuousServo motorVertical;

        ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
            int switchPin, bool normallyOpen);

        // Call in setup
        void begin(WireManager* wireManager);

        float getPosition();
        void setPosition(float angle);
        void testSequence();
        void homingSequence();
        void setAsHome();
        bool reachedTarget();
        void loop();
        void setPIDTuningMode(bool mode);
        void setPIDTuningPins(int P_Pin, int D_Pin);

    
};