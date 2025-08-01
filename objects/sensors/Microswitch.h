#pragma once
#include <Wire.h>
#include <Arduino.h>

// Local Libraries:


class Microswitch {
    private: 
        int switchPin;
        bool normallyOpen;
    public:
        Microswitch(int switchPin, bool normallyOpen);
        void begin();
        bool isPressed();
        
};