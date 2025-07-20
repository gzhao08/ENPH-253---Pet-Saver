#pragma once

#include <WireManager.h>
class WireManager;

class SensorI2C {
    private:
        WireManager* wireManager;
        int address;
        int muxLine;

    public:
        SensorI2C(int address, int muxLine=-1); // Default muxLine to -1 (no muxing)
        void assignWireManager(WireManager* wireManager);
        int beginTransmission();
        int endTransmission(bool stop=true);
        int requestFrom(int address, int quantity, bool stop=true);
        int available();
        int write(int value);
};