#pragma once
#include <Adafruit_VL53L0X.h>
extern volatile boolean drive;
extern portMUX_TYPE mux;
void objectDetected (void *parameter);
