#include "SharedState.h"

volatile boolean startRead = false;
volatile boolean drive = false;
volatile int currentSpeed = 0; // Current speed of the robot, can be used to adjust motor speeds dynamically
volatile unsigned long startMovementTime = 0;

void startReading() {
    startRead = true;
}

void stopReading() {
    startRead = false;
}

void stopDrive() {
    drive = false;
}

void startDrive() {
    drive = true;
}

void changeSpeed(int newSpeed) {
    currentSpeed = constrain(newSpeed,0,4096);
}

void recordStartTime() {
    startMovementTime = millis();
}