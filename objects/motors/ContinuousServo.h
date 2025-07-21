#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "driver/ledc.h"
#include "MagneticEncoder.h"
#include "DCMotor/DCMotor.h"

#define BIT_12_LIMIT 4096

class ContinuousServo {
    private: 
        // Should be multiplied to every duty cycle when driving the motor
        // to get the correct direction of rotation
        int DIRECTION_MULTIPLIER;
        DCMotor motor;

        MagneticEncoder encoder;

        // PID Controls
        float targetAngle;
        PID pidController;
        double Pk = 175;
        double Ik = 0;
        double Dk = 4;
        const int PIDSampleTime = 10;
        unsigned long lastPIDTime = 0;

        // PID Tuning
        bool PIDTuningMode = false;
        int P_Pin; // Pin for tuning P
        int D_Pin; // Pin for tuning D

        // Stability check
        const float tolerance = 3;
        const int stableThreshold = 20;
        int stableCounter = 0;

        void updateStability();
        void PIDSequence(float targetValue);
        void tunePID();
    public: 
        // PID Variables (needs to be public because referenced in PID controller)
        double Setpoint = 0, Input = 0, Output = 0;

        ContinuousServo(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide);

        void begin(WireManager* wireManager); // Call in setup
        void moveBy(float degrees);
        void moveTo(float degrees);
        void testSequence();
        bool reachedTarget();
        void home();
        float getAngle();
        void setPIDTuningMode(bool mode);
        void setPIDTuningPins(int P_Pin, int D_Pin);
        // Call in loop
        void loop();
        
};