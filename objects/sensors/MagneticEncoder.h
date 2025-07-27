#pragma once
#include <Wire.h>
#include <Arduino.h>

// Local Libraries:
#include "../managers/WireManager.h"
#include "../sensors/SensorI2C.h"
#include "../GlobalConstants.h"

class MagneticEncoder : public SensorI2C {
    private:
        // Relative angles
        float homeAngle = 0;
        float relAngle; // Angle relative to home
        float prevAngle;
        float currentAngle;

        uint16_t readRawAngle();
        void incrementRelAngle(float newAngle);
    public:
        MagneticEncoder(int muxLine=-1);
        void begin(WireManager* wireManagerObject);
        float readAngle();
        float angleDifference(float toAngle, float fromAngle);
        void setAsHome();
        float getRelAngle();
};
