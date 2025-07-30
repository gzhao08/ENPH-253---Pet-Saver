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
        // Arm
        static const int armMotorPin1 = 22;
        static const int armMotorPin2 = 19;
        static const int armPwmChannel1 = 15; //motor pin 1 goes to B
        static const int armPwmChannel2 = 14; 
        static const int armMuxLine = 1; 
        static const bool armEncoderOnTerminalSide = true; //false for arm, 
        static const int armSwitchPin = 37; 
        static const bool armNormallyOpen = true; 
        //muxLine: 1 is 1, 0 is 2, -1 is not muxing

        // Vertical stage
        static const int verticalStageMotorPin1 = 20;
        static const int verticalStageMotorPin2 = 21;
        static const int verticalStagePwmChannel1 = 4; //motor pin 1 goes to B
        static const int verticalStagePwmChannel2 = 5; 
        int verticalStageMuxLine = 0; 
        bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
        int verticalStageSwitchPin = 38; 
        bool verticalStageNormallyOpen = true; 

        // Base
        int baseMotorPin1 = 14;
        int baseMotorPin2 = 27;
        int basePwmChannel1 = 6; //motor pin 1 goes to B
        int basePwmChannel2 = 7; 
        int baseMuxLine = -1; 
        bool baseEncoderOnTerminalSide = false; //false for arm, 
        int baseSwitchPin = 9; 
        bool baseNormallyOpen = true; 

        // Grabber
        static const int grabberMotorPin = 12;
        static const int grabberPwmChannel = 1;
        //position in degrees
        float basePos = -1;
        float verticalPos = -1;
        float armPos = -1;
        float grabPos = -1;

    public: 
        ClawBase base;
        ClawVerticalStage vertical;
        ClawArm arm;
        ClawGrabber grabber;
        WireManager wireManager1;
        WireManager wireManager2;

        float LEFT_ANGLE; 
        float RIGHT_ANGLE; 

        ClawManager();

        // Call in setup
        void begin();
        void homingSequence();
        void reset();
        void depositLeft();
        void depositRight();
        void senseLeft(bool height);
        void senseRight(bool height);
        void retrieveLeft();
        void retrieveRight();
        void windowDrop(bool pos);
        void basketRelease();  
        
        void loop();
};