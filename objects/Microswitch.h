#pragma once
#include <Wire.h>
#include <Arduino.h>

class Microswitch {
    private: 
        int switchPin
        bool normallyOpen;
    public:
        Microswitch(int switchPin, bool normallyOpen);
        void begin();
        bool isPressed();
        bool justReleased();
        bool justPressed(); 
};