#include "SectionManager.h"
#include "../GlobalConstants.h"
#include <Adafruit_SSD1306.h>

#define CONSECUTIVE_THRESH 3 // number of consecutive measurements to consider a section change

SectionManager::SectionManager()
  : currentSection(0),
    objectCount(0),
    rightLidar(),
    leftLidar(),
    display(128, 64, &Wire, -1) {}

void SectionManager::begin(boolean useDisplay) {

    if (useDisplay) {
        while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println("SSD1306 allocation failed");
            delay(100);
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0);
        display.println("Initializing VL53L0X...");
        display.display();
    }

    Serial.println("Initializing Right VL53L0X...");
    while (!rightLidar.begin( (uint8_t) 41U, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
        Serial.println("Failed to boot Right VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Right VL53L0X Initialized!");

    Serial.println("Initializing Left VL53L0X...");
    while (!leftLidar.begin((uint8_t) 41U, false, &Wire1, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
        Serial.println("Failed to boot Left VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Left VL53L0X Initialized!");

    Serial.println("Both VL53L0X ready!");

    if (useDisplay) {
        display.clearDisplay();
        display.setTextSize(3);
        display.setCursor(0, 0);
        display.println("VL53L0X ready!");
        display.display();
        display.clearDisplay();
        display.setTextSize(3);
    }
}

boolean SectionManager::detectOutOfRange(bool useRight) {
    int distance;
    while (true) {
        useRight ? rightLidar.rangingTest(&rightMeasure, false) : leftLidar.rangingTest(&leftMeasure, false);
        distance = useRight ? rightMeasure.RangeMilliMeter : leftMeasure.RangeMilliMeter;
        if (distance >= 500) {
            return true;
        }
    }
}

boolean SectionManager::detect(bool useRight, int threshold, bool senseDir, int consecutiveCount) {
    int numConsecutive = 0;
    int distance;

    if (senseDir) {
        while (true) {
            useRight ? rightLidar.rangingTest(&rightMeasure, false) : leftLidar.rangingTest(&leftMeasure, false);
            distance = useRight ? rightMeasure.RangeMilliMeter : leftMeasure.RangeMilliMeter;
            if (distance <= threshold) {
                numConsecutive++;
                if (numConsecutive >= consecutiveCount) {
                    return true;
                }
            }
            else {
                numConsecutive = 0; // reset if not detecting
            }
        }
    }

    else {
        while (true) {
            useRight ? rightLidar.rangingTest(&rightMeasure, false) : leftLidar.rangingTest(&leftMeasure, false);
            distance = useRight ? rightMeasure.RangeMilliMeter : leftMeasure.RangeMilliMeter;
            if (distance >= threshold) {
                numConsecutive++;
                if (numConsecutive >= consecutiveCount) {
                    return true;
                }
            }
            else {
                numConsecutive = 0; // reset if not detecting
            }
        }
    }
    
    
}
