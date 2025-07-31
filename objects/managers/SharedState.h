#pragma once
#include <Arduino.h>

extern volatile boolean startRead; // Flag to indicate if reading should start
extern volatile boolean drive; // Flag to indicate if driving should start
extern volatile int currentSpeed;

void startRead();
void stopRead();    
void stopDrive();
void startDrive();
void changeSpeed(int speed);