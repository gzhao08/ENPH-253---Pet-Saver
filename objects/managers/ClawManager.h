#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

#include "../claw/ClawBase.h"
#include "../claw/ClawArm.h"
#include "../claw/ClawVerticalStage.h"
#include "../claw/ClawGrabber.h"

class ClawManager {
    private:
        ClawBase* base;
        ClawVerticalStage* vertical;
        ClawArm* arm;
        ClawGrabber* grab;

        //position in degrees
        float basePos = -1;
        float verticalPos = -1;
        float armPos = -1;
        float grabPos = -1;

    public: 

        float LEFT_ANGLE; 
        float RIGHT_ANGLE; 

        ClawManager();

        // Call in setup
        void begin(ClawBase* base, ClawVerticalStage* vertical, ClawArm* arm, ClawGrabber* grab);

        void reset();
        void depositLeft();
        void depositRight();
        void senseLeft(bool height);
        void senseRight(bool height);
        void retrieveLeft();
        void retrieveRight();
        void windowDrop(bool pos);
        void basketRelease();    
};