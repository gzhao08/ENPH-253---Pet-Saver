#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../sensors/MagneticEncoder.h"
#include "../motors/DCMotor.h"
#include "../managers/DelayManager.h"
#include "../managers/WireManager.h"

#define BIT_12_LIMIT 4096

class ContinuousServo {
    private: 
        // Should be multiplied to every duty cycle when driving the motor
        // to get the correct direction of rotation
        int DIRECTION_MULTIPLIER;

        MagneticEncoder encoder;

        // PID Controls
        float targetAngle; // Target angle for PID
        PID* pidController;
        // Default PID Values
        double Pk = 110;
        double Ik = 0;
        double Dk = 2.5;
        const int PIDSampleTime = 10;
        unsigned long lastPIDTime = 0;

        // PID Tuning
        DelayManager pidTuningDelayManager;
        bool PIDTuningMode = false;
        int P_Pin; // Pin for tuning P
        int D_Pin; // Pin for tuning D

        // Stability check
        const int stableThreshold = 20;
        int stableCounter = 0;

        void updateStability();
        void PIDSequence(float targetValue);
        void tunePID();
        unsigned long lastPrint = 0;
    public: 
        DCMotor motor;

        // PID Variables (needs to be public because referenced in PID controller)
        double Setpoint = 0, Input = 0, Output = 1;
        bool logPIDOutput = false;

        // Angle tolerance (how much can deviate from target angle)
        float tolerance = 6;

        ContinuousServo(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int maxVoltage = 5);

        void begin(WireManager* wireManager); // Call in setup
        void moveBy(float degrees);
        void moveTo(float degrees);
        bool reachedTarget();
        void setAsHome();
        float getAngle();

        // PID Stuff
        void setPIDTuningMode(bool mode);
        void setPIDTuningPins(int P_Pin, int D_Pin);
        void setPDTuning(float Kp, float Kd); // Set the PD tuning values

        void setMaxVoltage(int voltage); // Set the voltage of the motor, which will update the max duty cycle
        // Call in loop
        void loop();

        void testSequence();

        
};