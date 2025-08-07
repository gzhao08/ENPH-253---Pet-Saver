#include "SharedState.h"

volatile boolean startRead = false;
volatile boolean drive = false;
volatile int currentSpeed = 0; // Current speed of the robot, can be used to adjust motor speeds dynamically
volatile unsigned long startMovementTime = 0;

volatile RobotState robotState = RobotState::IDLE;
portMUX_TYPE robotStateMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE startTimeMux = portMUX_INITIALIZER_UNLOCKED;

// TaskHandle_t drivetrainTaskHandle;


void stopDrive() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = STOPPED;
    portEXIT_CRITICAL(&robotStateMux);
}

void startLineFollow() {
    // xTaskNotifyGive(drivetrainTaskHandle);
    portENTER_CRITICAL(&robotStateMux);
    robotState = LINE_FOLLOW;
    portEXIT_CRITICAL(&robotStateMux);
}

void changeSpeed(int newSpeed) {
    currentSpeed = newSpeed;
}

void recordStartTime() {
    portENTER_CRITICAL(&startTimeMux);
    startMovementTime = millis();
    portEXIT_CRITICAL(&startTimeMux);
}

void startForward() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = FORWARD;
    portEXIT_CRITICAL(&robotStateMux);
}

void turnCW() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = TURN_CW;
    portEXIT_CRITICAL(&robotStateMux);
}

void turnCCW() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = TURN_CCW;
    portEXIT_CRITICAL(&robotStateMux);
}

void turnCW_Back() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = TURN_CW_BACK;
    portEXIT_CRITICAL(&robotStateMux);
    // xTaskNotifyGive(drivetrainTaskHandle);
}

void startBackward() {
    portENTER_CRITICAL(&robotStateMux);
    robotState = BACKWARD;
    portEXIT_CRITICAL(&robotStateMux);
}