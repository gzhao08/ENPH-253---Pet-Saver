#pragma once

#include "../motors/Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class Basket {
    private:
        MyServo motor;
        
    public: 
        Basket(int servoPin, int pwmChannel);

        // Call in setup
        void begin();

        void reset();
        void release(); 
    
};