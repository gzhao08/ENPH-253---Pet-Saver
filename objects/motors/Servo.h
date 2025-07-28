#pragma once
#include <Wire.h>
#include <Arduino.h>
#include "driver/ledc.h"

// Local Libraries:

class MyServo {
    private:
        int motorPin; 
        int pwmChannel;
        int servoFreq = 50; 

        int dutyCycle = -1; 
        int positionDegrees = -1;

        // Duty cycle (test)
        int MIN_PERCENT = 3; 
        int MAX_PERCENT = 13; 

        const int MIN_POSITION_DEG = 0;
        const int MAX_POSITION_DEG = 180;

        // Calibrated for MG966R
        // Pulse duration for min and max positions
        const float SERVO_PERIOD_MS = 1000 / servoFreq;
        const float MIN_POSITION_PULSE_MS = 0.500;
        const float MAX_POSITION_PULSE_MS = 2.500;

        // PWM Duty cycle for min and max positions
        int MIN_POSITION_PWM = MIN_POSITION_PULSE_MS / SERVO_PERIOD_MS * 4096;
        int MAX_POSITION_PWM = MAX_POSITION_PULSE_MS / SERVO_PERIOD_MS * 4096;

        int RANGE_DEG = MAX_POSITION_DEG - MIN_POSITION_DEG;
        int RANGE_POS = MAX_POSITION_PWM - MIN_POSITION_PWM; // pwm
        int RANGE_DUTY = MAX_PERCENT - MIN_PERCENT;
        
    public:
        bool logOutput = false; // Enable serial print
        MyServo(int motorPin, int pwmChannel);
        void begin();
        void writePosition(int angle);
        void writeDutyCycle(float duty);
        float readPosition();
        float readDutyCycle();
};
