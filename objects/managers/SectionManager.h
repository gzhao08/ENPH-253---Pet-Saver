#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SharedState.h"

class SectionManager {
    private:
        int currentSection; // 0, 1, or 2
        int objectCount;

        Adafruit_VL53L0X rightLidar;
        Adafruit_VL53L0X leftLidar;
        VL53L0X_RangingMeasurementData_t rightMeasure;
        VL53L0X_RangingMeasurementData_t leftMeasure;
        Adafruit_SSD1306 display;


    public:
        SectionManager(boolean useDisplay);

        void begin();

        int currentSection() {
            return getCurrentSection;
        }

        boolean detectOutOfRange(int distance);
        boolean detect(int distance, int threshold, bool senseDir, int consecutiveCount);
};



