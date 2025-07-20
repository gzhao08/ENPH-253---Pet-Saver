#pragma once

#include <Arduino.h>
#include <Wire.h>

class WireManager {
    private:
        TwoWire* wire; // The Wire object to use for I2C communication
        int muxPin;

        void switchMuxLine(int muxLine);
    public:
        WireManager(int muxPin);

        void begin(TwoWire* wireObj);
        int beginTransmission(int muxLine, int address);
        int endTransmission(int muxLine, bool stop = true);
        int requestFrom(int muxLine, int address, int quantity, bool stop = true);
        int available(int muxLine);
        int write(int muxLine, int value);
};