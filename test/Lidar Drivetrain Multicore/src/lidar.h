#pragma once
#include <Adafruit_VL53L0X.h>
extern volatile boolean drive;
extern portMUX_TYPE mux;
extern volatile boolean startRead;
void objectDetected (void *parameter);
