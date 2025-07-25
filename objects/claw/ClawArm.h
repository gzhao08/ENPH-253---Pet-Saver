#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawArm {
    private:
        Microswitch mswitchArm;

        const int HORIZONTAL_GEAR_CIRCUMFERENCE = 151; // mm
        const float ANGLE_TO_MM_CONVERSION = -0.42; // Calibrated from 151/360
        const float MM_TO_ANGLE_CONVERSION = 1 / ANGLE_TO_MM_CONVERSION; // 1mm corresponds to 1/0.42 degrees
        
    public: 
        ContinuousServo motorArm;

        const int MIN_POSITION = 0;
        const int MAX_POSITION = 200;
        ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen);

        // Call in setup
        void begin(WireManager* wireManager);

        void setAsHome();
        void homingSequence();
        float reachedTarget();

        float getPosition();
        void setPosition(float distance);
        void testSequence();

        void setPIDTuningMode(bool mode);
        void setPIDTuningPins(int P_Pin, int D_Pin);
        void loop();

};