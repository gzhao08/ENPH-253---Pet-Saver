#pragma once

#include "Servo.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class Basket {
    private:
        Servo* motor;
        
    public: 
        Basket();

        // Call in setup
        void begin(Servo* motor);

        void reset();
        void release(); 
    
};