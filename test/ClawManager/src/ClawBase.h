#pragma once

#include "ContinuousServo.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawBase {
    private:
        ContinuousServo* motor;
        //position in degrees
        int motorPin1;
        int motorPin2;
        float position;

    public: 

        ClawBase();

        // Call in setup
        void begin(ContinuousServo* motor);

        float getPosition();
        void setPosition(float angle);
        void testSequence();

        // Call in loop
        void loop();       
};