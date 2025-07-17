#pragma once
#include <Wire.h>

class MagneticEncoder {
    private:
        TwoWire* wire; // Will have to mux
        const int AS5600_ADDR = 0x36;
        const int AS5600_MSG_REG = 0x0C;

    public:
        MagneticEncoder();
        void begin(TwoWire* wireObj);
        uint16_t readRawAngle();
        double readAngle();
        double angleDifference(double toAngle, double fromAngle);
};
