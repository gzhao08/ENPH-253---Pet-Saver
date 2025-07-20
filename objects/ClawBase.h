#pragma once

#include "ContinuousServo.h"
#include "Microswitch.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawBase {
    private:
        ContinuousServo* motor;
        Microswitch* mswitch;

        //position in degrees
        float position = -1;
        int MIN_POSITION = -120;
        int MAX_POSITION = 120;

    public: 

        ClawBase();

        // Call in setup
        void begin(ContinuousServo* motor, Microswitch* mswitch);

        float getPosition();
        void setPosition(float angle);
        void testSequence();
    
};