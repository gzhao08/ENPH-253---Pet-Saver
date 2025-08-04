#pragma once
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

extern volatile boolean startRead; // Flag to indicate if reading should start
extern volatile boolean drive; // Flag to indicate if driving should start
extern volatile int currentSpeed;
extern volatile unsigned long startMovementTime;

enum RobotState {
    LINE_FOLLOW,
    SENSING,
    STOPPED,
    TURN_CW,
    TURN_CCW,
    FORWARD,
    BACKWARD,
    TURN_CW_BACK,
    IDLE
};

extern volatile RobotState robotState;
extern portMUX_TYPE robotStateMux;

void recordStartTime();
void startLineFollow();
void changeSpeed(int speed);
void stopDrive();
void startForward();
void turnCW();
void turnCW_Back();
void turnCCW();
void startBackward();
