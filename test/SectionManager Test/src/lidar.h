#pragma once
#include <Adafruit_VL53L0X.h>
#include "SharedState.h"

void objectDetected (void *parameter);

void clawSeq1Pet1();
void clawSeq2Ramp();
void clawSeq3Pet3();
