#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"
#include <Adafruit_LIS3MDL.h>

#include "../claw/ClawBase.h"
#include "../claw/ClawArm.h"
#include "../claw/ClawVerticalStage.h"
#include "../claw/ClawGrabber.h"

class ClawManager {
    // private:
    //     // Arm
    //     static const int armMotorPin1 = 22;
    //     static const int armMotorPin2 = 19;
    //     static const int armPwmChannel1 = 15; //motor pin 1 goes to B
    //     static const int armPwmChannel2 = 14; 
    //     static const int armMuxLine = 1; 
    //     static const bool armEncoderOnTerminalSide = true; //false for arm, 
    //     static const int armSwitchPin = 37; 
    //     static const bool armNormallyOpen = true; 
    //     //muxLine: 1 is 1, 0 is 2, -1 is not muxing

    //     // Vertical stage
    //     static const int verticalStageMotorPin1 = 20;
    //     static const int verticalStageMotorPin2 = 21;
    //     static const int verticalStagePwmChannel1 = 4; //motor pin 1 goes to B
    //     static const int verticalStagePwmChannel2 = 5; 
    //     int verticalStageMuxLine = 0; 
    //     bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
    //     int verticalStageSwitchPin = 38; 
    //     bool verticalStageNormallyOpen = true; 

    //     // Base
    //     int baseMotorPin1 = 14;
    //     int baseMotorPin2 = 27;
    //     int basePwmChannel1 = 6; //motor pin 1 goes to B
    //     int basePwmChannel2 = 7; 
    //     int baseMuxLine = -1; 
    //     bool baseEncoderOnTerminalSide = false; //false for arm, 
    //     int baseSwitchPin = 9; 
    //     bool baseNormallyOpen = true; 

    //     // Grabber
    //     static const int grabberMotorPin = 12;
    //     static const int grabberPwmChannel = 1;
    //     //position in degrees
    //     float basePos = -1;
    //     float verticalPos = -1;
    //     float armPos = -1;
    //     float grabPos = -1;

    public:
        ClawManager(); 
        ClawBase base;
        ClawVerticalStage vertical;
        ClawArm arm;
        ClawGrabber grabber;
        WireManager wireManager1;
        WireManager wireManager2;
        Adafruit_LIS3MDL lis3mdl;


        float HARD_IRON_OFFSET_X = 0;
        float HARD_IRON_OFFSET_Y = 0;
        float HARD_IRON_OFFSET_Z = 0;


        //POSITIONS
        //Base
        float BASE_FORWARD = 0;
        float BASE_LEFT_BASKET = 0;
        float BASE_RIGHT_BASKET = 30;
        float BASE_LEFT = -90; //left off ramp 
        float BASE_RIGHT = 90; 
        //Vertical
        float VERTICAL_MIN = 0; 
        float VERTICAL_PICKUP_LOW = -10;
        float VERTICAL_DEBRIS_HEIGHT = 60; //use every time pet is picked up, 2.5"
        float VERTICAL_PET_ABOVE_CHASSIS = 30; //CHECK
        float VERTICAL_LOW_SCAN = 40; //CHECK
        float VERTICAL_HIGH_SCAN = 80; //CHECk
        float VERTICAL_MAX = 130; //CHECK
        //Horizontal
        float ARM_MIN = 0; 
        float ARM_PICKUP_LOW = -10; 
        float ARM_MAX = 227; 
        float ARM_HIGH_GRAB = 10;
        float ARM_OUTSIDE_CHASSIS = 60;
        //Grabber
        float GRABBER_CLOSE = 20;
        float GRABBER_BASKET_OPEN = 50;
        float GRABBER_CHUTE_OPEN = 22;
        float GRABBER_OPEN = 100;

        bool LEFT = 0; 
        bool RIGHT = 1;

        // Call in setup
        void begin();
        void homingSequence();
        void reset(); 

        void sensePet(); 
        float getMagnetReadingMagSq();
        void calibrateMagnet();

        void waitToReachTarget(int timeout=-1);


        void stopAll();
        void seqDrivePosition(float height);
        void seqPickUpLow();
        void seqRetract();
        void seqRampDrop();
        void seqDeposit(bool direction);
        void seqRetrieve(bool direction);
        void seqPickUpHigh(bool chute);
        void seqWindowDrop(bool direction);
        
        void setPositionArm(int pos);
        void setPositionVertical(int pos);
        void setPositionBase(int pos);
        void setPositionGrabber(int pos);

        void loop();
};