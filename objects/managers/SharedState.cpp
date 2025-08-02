#include "SharedState.h"

volatile boolean startRead = false;
volatile boolean drive = false;
volatile int currentSpeed = 0; // Current speed of the robot, can be used to adjust motor speeds dynamically
volatile unsigned long startMovementTime = 0;

volatile RobotState robotState = RobotState::LINE_FOLLOW;
portMUX_TYPE robotStateMux = portMUX_INITIALIZER_UNLOCKED;


void stopDrive() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = STOPPED;
    portEXIT_CRITICAL(&robotStateMux);
}

void startLineFollow() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = LINE_FOLLOW;
    portEXIT_CRITICAL(&robotStateMux);
}

void changeSpeed(int newSpeed) {
    currentSpeed = newSpeed;
}

void recordStartTime() {
    startMovementTime = millis();
}

void startForward() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = FORWARD;
    portEXIT_CRITICAL(&robotStateMux);
}