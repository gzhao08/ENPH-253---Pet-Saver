#include "SharedState.h"

volatile boolean startRead = false;
volatile boolean drive = false;
volatile int currentSpeed = 0; // Current speed of the robot, can be used to adjust motor speeds dynamically

void startRead() {
    startRead = true;
}

void stopRead() {
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