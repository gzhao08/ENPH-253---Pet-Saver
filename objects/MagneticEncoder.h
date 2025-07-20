#pragma once
#include <Wire.h>
#include <Arduino.h>
#include <WireManager.h>
#include <SensorI2C.h>

class MagneticEncoder : public SensorI2C {
    private:
        // I2C stuff
        const int AS5600_ADDR = 0x36;
        const int AS5600_MSB_REG = 0x0C;

        // Relative angles
        float homeAngle = 0;
        float relAngle; // Angle relative to home
        float prevAngle;
        float currentAngle;

        void incrementRelAngle(float newAngle);
    public:
        MagneticEncoder(int muxLine=-1);
        void begin(WireManager* wireManagerObject);
        uint16_t readRawAngle();
        float readAngle();
        float angleDifference(float toAngle, float fromAngle);
        void home();
        float getRelAngle();
};
