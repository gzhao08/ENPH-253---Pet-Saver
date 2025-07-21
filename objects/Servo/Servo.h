#pragma once
#include <Wire.h>
#include <Arduino.h>
#include "driver/ledc.h"

class Servo {
    private:
        int motorPin; 
        int pwmChannel;
        int servoFreq = 50; 

        int dutyCycle = -1; 
        int positionDegrees = -1;

        int MIN_PERCENT = 3; 
        int MAX_PERCENT = 13; 

        int RANGE_DEG = 180;
        int RANGE_POS = 410;
        int RANGE_DUTY = 10;

        int MIN_POSITION = 122; //0 degrees
        int MAX_POSITION = 533; //180 degrees   
        
    public:
        Servo(int motorPin, int pwmChannel);
        void begin();
        void writePosition(int angle);
        void writeDutyCycle(float duty);
        float readPosition();
        float readDutyCycle();
};
