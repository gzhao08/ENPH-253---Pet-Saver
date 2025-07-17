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

        PID* pidController;
        double Pk = 180;
        double Ik;
        double Dk = 5;
    public: 
        // PID Variables
        double Setpoint = 0, Input = 0, Output = 0;

        ContinuousServo(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2);

        // Call in setup
        void begin(MagneticEncoder* enc);

        void drivePWM(int signedDuty);
        void moveBy(int degrees);
        void moveTo(int degrees);
        void PIDSequence(int targetValue);
        void testSequence();

        // Call in loop
        void loop();
        
};