#pragma once

#include <Arduino.h>
#include "driver/ledc.h"

class DCMotor {
    private:
        int motorPin1; // Pin controlling left terminal of the motor
        int motorPin2; // Pin controlling right terminal of the motor
        int pwmChannel1; // PWM channel for motorPin1
        int pwmChannel2; // PWM channel for motorPin2
        int maxVoltage; // Maximum voltage for the motor
    public:
        DCMotor(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int maxVoltage=5);
        void begin();
        void drivePWM(int signedDuty);
        void stop();
        void setSpeed(int speed); // Speed in percentage (0-100)
        int getSpeed(); // Returns the current speed in percentage
}