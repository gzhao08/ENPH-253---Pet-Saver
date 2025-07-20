#pragma once

#include "ClawBase.h"
#include "ClawArm.h"
#include "ClawVerticalStage.h"
#include "ClawGrabber.h"
#include "Basket.h"
#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"


class ClawManager {
    private:
        ClawBase* base;
        ClawVerticalStage* vertical;
        ClawArm* arm;
        ClawGrabber* grab;
        Basket* basket;

        //position in degrees
        float basePos = -1;
        float verticalPos = -1;
        float armPos = -1;
        float grabPos = -1;

    public: 
        ClawManager();

        // Call in setup
        void begin(ClawBase* base, ClawVerticalStage* vertical, ClawArm* arm, ClawGrabber* grab, Basket* basket);

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