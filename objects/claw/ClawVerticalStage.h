#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"
#include "../claw/ClawPart.h"

class ClawVerticalStage : public ClawPart {
    private:
        String _partName = "Claw Vertical Stage";

        const float _ENCODER_TO_POS_CONVERSION = -0.022; /// Calibrating from 2mm pitch * 4 starts / 360 degrees
        const float _POS_TO_ENCODER_CONVERSION = 1 / _ENCODER_TO_POS_CONVERSION; // 1mm corresponds to 1/0.42 degrees
        const int _ABS_POS_LIMIT = 130;
        const int _MIN_POSITION = 0;
        const int _MAX_POSITION = 120;

        // PID Parameters
        const float _Pk = 55;
        const float _Dk = 0.77;

        // Servo parameters
        const int _servoMaxVoltage = 8;
        const int _servoTolerance = 20; // How much can angle deviate from target

        const int IDLE_HEIGHT = 50;

        const int LOW_PET_HEIGHT = 10;
        const int HIGH_PET_HEIGHT = 90;
        const int LOW_PET_SENSING_HEIGHT = 20;
        const int HIGH_PET_SENSING_HEIGHT = 80;
    public: 
        ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
            int switchPin, bool normallyOpen);
        void testSequence();
};