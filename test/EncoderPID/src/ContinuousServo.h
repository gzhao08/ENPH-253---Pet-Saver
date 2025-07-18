#pragma once

#include "MagneticEncoder.h"
#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "driver/ledc.h"

#define BIT_12_LIMIT 4096

class ContinuousServo {
    private: 
        // Pin 1 should be connected to controller of left terminal of motor
        // (terminal is facing you)
        int motorPin1;
        int motorPin2;
        // PWM Channel linked to motorPins
        int pwmChannel1;
        int pwmChannel2;

        MagneticEncoder* encoder;

        // PID Controls
        float targetAngle;
        PID* pidController;
        double Pk = 180;
        double Ik = 0;
        double Dk = 1;
        const int PIDSampleTime = 10;
        unsigned long lastPIDTime = 0;


        // Stability check
        const float tolerance = 3;
        const int stableThreshold = 20;
        int stableCounter = 0;

        void updateStability();
        void PIDSequence(float targetValue);


    public: 
        // PID Variables (needs to be public because referenced in PID controller)
        double Setpoint = 0, Input = 0, Output = 0;

        ContinuousServo(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2);

        // Call in setup
        void begin(MagneticEncoder* enc);

        void drivePWM(int signedDuty);
        void moveBy(float degrees);
        void moveTo(float degrees);
        void testSequence();
        bool reachedTarget();

        // Call in loop
        void loop();
        
};