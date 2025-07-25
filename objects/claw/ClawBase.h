#pragma once


#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawBase {
    private:
        Microswitch mswitchBase;

        const int HORIZONTAL_GEAR_CIRCUMFERENCE = 151; // mm
        const float ENCODER_TO_BASE_ANGLE_CONVERSION = -0.4; // Calibrated from 6/15
        const float BASE_TO_ENCODER_ANGLE_CONVERSION = 1 / ENCODER_TO_BASE_ANGLE_CONVERSION; // 1mm corresponds to 1/0.42 degrees

        const int MIN_BASE_ANGLE_POSITION = -120;
        const int MAX_BASE_ANGLE_POSITION = 120;

    public: 
        ContinuousServo motorBase;

        ClawBase(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen);

        // Call in setup
        void begin(WireManager* wireManager);

        void setAsHome();
        void homingSequence();
        float reachedTarget();

        float getPosition();
        void setPosition(float position);
        void testSequence();

        void loop();
};