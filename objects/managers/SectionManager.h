#pragma once
#include <Arduino.h>
// #include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SharedState.h"
#include <VL53L0X.h>

class SectionManager {
    private:
        int currentSection;
        int objectCount;
        bool useDisplay;
        int numConsecutive;
        unsigned long detectionTime;

        VL53L0X rightLidar;
        VL53L0X leftLidar;

        

    public:
        Adafruit_SSD1306 display;

        enum Section {
            DOORWAY,
            PET_1,
            RAMP,
            RAMP_END,
            WINDOW_FORWARD,
            PET_3,
            PET_4,
            WINDOW_BACKWARD
        };

        SectionManager();

        void begin(boolean useDisplay);

        int getCurrentSection() {
            return currentSection;
        }

        void incrementObjectCount() {
            objectCount++;
        }

        void incrementSection() {
            currentSection++;
        }

        boolean detectOutOfRange(bool useRight);
        boolean detectCloser(bool useRight, int threshold, int consecutiveCount);
        boolean detectFurther(bool useRight, int threshold, int consecutiveCount);
        boolean show(String message);
        void getNextSection();
        void setDetectionTime();
};



