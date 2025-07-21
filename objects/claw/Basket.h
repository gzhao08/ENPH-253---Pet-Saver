#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Our libraries
#include "../motors/Servo.h"

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