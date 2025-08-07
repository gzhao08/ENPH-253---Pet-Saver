#pragma once
#include <Arduino.h>
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
            PET_3,
            RAMP_END_BACK,
            WINDOW_BACKWARD,
            WINDOW_FORWARD,
            PET_5
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
        int getMeasurement(bool useRight);
        void getNextSection();
        void setDetectionTime();
};



