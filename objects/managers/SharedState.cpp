#include "SharedState.h"

volatile boolean startRead = false;
volatile boolean drive = false;
volatile int currentSpeed = 0; // Current speed of the robot, can be used to adjust motor speeds dynamically