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
        int MAX_PERCENT = 12; 
        int MIN_POSITION = 123; //0 degrees
        int MAX_POSITION = 492; //180 degrees   
        
    public:
        Servo(int motorPin, int pwmChannel);
        void begin();
        void writePosition(int angle);
        void writeDutyCycle(float duty);
        float readPosition();
        float readDutyCycle();
};
