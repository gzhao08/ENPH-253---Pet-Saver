#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SharedState.h"

class SectionManager {
    private:
        int currentSection;
        int objectCount;
        bool useDisplay;

        Adafruit_VL53L0X rightLidar;
        Adafruit_VL53L0X leftLidar;
        VL53L0X_RangingMeasurementData_t rightMeasure;
        VL53L0X_RangingMeasurementData_t leftMeasure;
        Adafruit_SSD1306 display;

    public:
        SectionManager();

        void begin(boolean useDisplay);

        int getCurrentSection() {
            return currentSection;
        }

        void incrementObjectCount() {
            objectCount++;
        }

        boolean detectOutOfRange(bool useRight);
        boolean detectCloser(bool useRight, int threshold, int consecutiveCount);
        boolean detectFurther(bool useRight, int threshold, int consecutiveCount);
        boolean show(String message);
};



