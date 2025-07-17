#pragma once
#include <Wire.h>
#include <Arduino.h>

class MagneticEncoder {
    private:
        // I2C stuff
        TwoWire* wire; // Will have to mux
        const int AS5600_ADDR = 0x36;
        const int AS5600_MSG_REG = 0x0C;

        // Relative angles
        float homeAngle = 0;
        float relAngle; // Angle relative to home
        float prevAngle;
        float currentAngle;

        void incrementRelAngle(float newAngle);
    public:
        MagneticEncoder();
        void begin(TwoWire* wireObj);
        uint16_t readRawAngle();
        float readAngle();
        float angleDifference(float toAngle, float fromAngle);
        void home();
        float getRelAngle();
};
