#pragma once

#include "WireManager.h"
class WireManager;

class SensorI2C {
    private:
        WireManager* wireManager;
        int address;
        int muxLine;

    public:
        SensorI2C(int address, int muxLine=-1); // Default muxLine to -1 (no muxing)
        void assignWireManager(WireManager* wireManager);
        void beginTransmission();
        int endTransmission(bool stop=true);
        int requestFrom(int address, int quantity, int stop=true);
        int available();
        int write(int value);
        uint8_t read();
};