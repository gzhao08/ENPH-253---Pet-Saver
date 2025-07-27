#pragma once

#include <Arduino.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../GlobalConstants.h"

class DCMotor {
    private:
        int motorPin1; // Pin controlling left terminal of the motor
        int motorPin2; // Pin controlling right terminal of the motor
        int pwmChannel1; // PWM channel for motorPin1
        int pwmChannel2; // PWM channel for motorPin2

        const int hBridgeVoltage = 12; // Voltage of the H-Bridge, used to calculate max duty cycle
        int maxVoltage; // Maximum voltage for the motor
        int maxDutyCycle; // Max duty cycle of DC motor. This will affect drivePWM

        // PWM stuff (ledc setup)
        const int pwmFrequency = 100;
        const int pwmResolution = 12;
        void updateMaxDutyCycle();

    public:
        DCMotor(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int maxVoltage=6);
        void begin();
        void drivePWM(int signedDuty);
        void stop();
        int getMaxDutyCycle();
        void setMaxVoltage(int voltage);
};