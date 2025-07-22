#pragma once

#include <Arduino.h>
#include <Wire.h>

// Local Libraries:

class WireManager {
    private:
        TwoWire* wire; // The Wire object to use for I2C communication
        int muxPin;

        void switchMuxLine(int muxLine);
    public:
        WireManager(int muxPin);

        void begin(TwoWire* wireObj);
        void beginTransmission(int muxLine, int address);
        int endTransmission(int muxLine, bool stop = true);
        int requestFrom(int muxLine, int address, int quantity, int stop = true);
        int available(int muxLine);
        int write(int muxLine, int value);
        uint8_t read(int muxLine);
};