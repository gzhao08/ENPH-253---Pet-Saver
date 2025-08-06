#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"
#include "../claw/ClawPart.h"

class ClawArm : public ClawPart {
    private:
        String _partName = "Claw Arm";

        const float _ENCODER_TO_POS_CONVERSION = -0.429; // Calibrated from 151/360
        const float _POS_TO_ENCODER_CONVERSION = 1 / _ENCODER_TO_POS_CONVERSION; // 1mm corresponds to 1/0.42 degrees
        const int _ABS_POS_LIMIT = 200;
        const int _MIN_POSITION = 0;
        const int _MAX_POSITION = 195;

        // PID Parameters
        const float _Pk = 35;
        const float _Dk = 3;

        // Servo parameters
        const int _servoMaxVoltage = 6;
        const int _servoTolerance = 10; // How much can angle deviate from target
    public: 
        ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
            int switchPin, bool normallyOpen);

        void testSequence();
};