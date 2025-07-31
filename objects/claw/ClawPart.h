#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "driver/ledc.h"

// Local Libraries:
#include "../motors/ContinuousServo.h"
#include "../sensors/Microswitch.h"

class ClawPart {
    protected:
        String partName; // For debugging purposes

        // Conversion and positions
        float ENCODER_TO_POS_CONVERSION;
        float POS_TO_ENCODER_CONVERSION;

        // PID Parameters
        float Pk;
        float Dk;

        // Servo Parameters
        int servoMaxVoltage;
        int servoTolerance; // How much can angle deviate from target

        // Speed stuff
        float lastPosition;
        unsigned long lastPositionTime;
        float speed;

        void updateSpeed();
    public:
        int ABS_POS_LIMIT; // Position limit (for homing purposes)
        int MIN_POSITION;
        int MAX_POSITION;

        Microswitch microswitch;
        ContinuousServo continuousServo;

        ClawPart(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
            int switchPin, bool normallyOpen);
    
        // Call in setup
        void begin(WireManager* wireManager);

        float getPosition();
        void setPosition(float position);
        void moveBy(float movement);
        bool reachedTarget();
        float getSpeed();

        void homingSequence();
        void setAsHome();

        void setPIDTuningMode(bool mode);
        void setPIDTuningPins(int P_Pin, int D_Pin);

        void loop();
    };