#pragma once

#include "ContinuousServo.h"
#include "Microswitch.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawVerticalStage {
    private:
        ContinuousServo* motor;
        Microswitch* mswitch;

        float height = -1; //height in mm
        int pitch = 2; // pitch in mm

    public: 

        int MIN_HEIGHT = 0;
        int MAX_HEIGHT;
        int RESET_HEIGHT; 
        int LOW_PET; 
        int HIGH_PET; 
        int LOW_SENSE; 
        int HIGH_SENSE; 

        ClawVerticalStage();

        // Call in setup
        void begin(ContinuousServo* motor, Microswitch* mswitch);

        float getPosition();
        void setPosition(float angle);
        void testSequence();
        void home();
    
};