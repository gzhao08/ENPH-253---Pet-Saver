#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "../managers/WireManager.h"
#include "../managers/DelayManager.h"
#include "../motors/Servo.h"
#include "../motors/ContinuousServo.h"
#include "../claw/ClawGrabber.h"
#include "../claw/ClawArm.h"
#include "../claw/ClawVerticalStage.h"
#include "../claw/ClawBase.h"

#include "../sensors/Microswitch.h"
#include "../GlobalConstants.h"
#include <Adafruit_LIS3MDL.h>

class ClawTester {
    public:
        void use();
};