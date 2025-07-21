#pragma once


#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawBase {
    private:
        ContinuousServo* motor;
        Microswitch* mswitch;

        //position in degrees
        float position = -1;
        float MIN_POSITION = -120;
        float MAX_POSITION = 120;

    public: 

        ClawBase();

        // Call in setup
        void begin(ContinuousServo* motor, Microswitch* mswitch);

        void home();
        float getPosition();
        void setPosition(float angle);
        void testSequence();
    
};