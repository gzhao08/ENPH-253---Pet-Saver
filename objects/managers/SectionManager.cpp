#include "SectionManager.h"
#include "../GlobalConstants.h"
#include <Adafruit_SSD1306.h>

/*
 * SectionManager.cpp
 * 
 * This file contains the implementation of the SectionManager class, which manages
 * the sections of the pet saver project, including lidar sensors and display.
 * 
 * ------------------------- NONE OF CODE CAN BE BLOCKING ------------------------
 */

SectionManager::SectionManager()
  : currentSection(0),
    objectCount(0),
    numConsecutive(0),
    detectionTime(0),
    useDisplay(false),
    rightLidar(),
    leftLidar(),
    display(128, 64, &Wire, -1) {}

void SectionManager::begin(boolean useDisplay) {

    if (useDisplay) {
        this->useDisplay = true;
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

    rightLidar.setBus(&Wire);
    leftLidar.setBus(&Wire1);

    Serial.println("Initializing Right VL53L0X...");
    while (!rightLidar.init()) {
        Serial.println("Failed to boot Right VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Right VL53L0X Initialized!");

    Serial.println("Initializing Left VL53L0X...");
    while (!leftLidar.init()) {
        Serial.println("Failed to boot Left VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Left VL53L0X Initialized!");

    Serial.println("Both VL53L0X ready!");

    leftLidar.startContinuous();
    rightLidar.startContinuous();
    

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
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    if (distance >= 500) {
        return true;
    }
    return false;
}

boolean SectionManager::detectCloser(bool useRight, int threshold, int consecutiveCount) {
    
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    if (useDisplay) {
        display.clearDisplay();
        display.setCursor(50, 25);
        display.setTextSize(3);
        display.println(distance);
        display.display();
    }
    if (distance <= threshold) {
        numConsecutive++;
        if (numConsecutive >= consecutiveCount) {
            numConsecutive = 0; // reset after detecting
            return true;
        }
    }
    else {
        numConsecutive = 0; // reset if not detecting
    }
    return false;
}

boolean SectionManager::detectFurther(bool useRight, int threshold, int consecutiveCount) {
    int numConsecutive = 0;

    // if (useDisplay) {
    //     display.clearDisplay();
    //     display.setCursor(0, 0);
    //     display.setTextSize(2);
    //     display.print(useRight ? "Detecting Right..." : "Detecting Left...");
    //     display.display();
    // }

    
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    if (useDisplay) {
        display.clearDisplay();
        display.setCursor(50, 25);
        display.setTextSize(3);
        display.println(distance);
        display.display();
    }
    if (distance >= threshold) {
        numConsecutive++;
        if (numConsecutive >= consecutiveCount) {
            numConsecutive = 0; // reset after detecting
            return true;
        }
    }
    else {
        numConsecutive = 0; // reset if not detecting
    }
        
    return false;
}

boolean SectionManager::show(String message) {
    if (useDisplay) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println(message);
        display.display();
        return true;
    }
    return false;
}

void SectionManager::getNextSection(){
    switch(getCurrentSection()) {

        case SectionManager::DOORWAY: {
            // looking for doorway
            if (detectCloser(false, 120, 1)) {
                detectionTime = millis();
                incrementSection();
                stopDrive();
            }
            break;
        }

        case SectionManager::PET_1: {
            if (millis() - detectionTime > 1000) {
                if (detectCloser(true, 250, 2)) {
                    detectionTime = millis();
                    incrementSection();
                    stopDrive();
                    show("STOP");
                    delay(2000);
                }
            }
            
            break;
        }

      case SectionManager::RAMP: {
        if (millis() - detectionTime > 1000) {
                if (detectCloser(true, 250, 3)) {
                    detectionTime = millis();
                    incrementSection();
                    stopDrive();
                    show("STOP");
                    delay(2000);
                }
            }
        break;
      }

      case SectionManager::RAMP_END: {
        if (millis() - detectionTime > 1500) {
                if (detectCloser(false, 250, 3)) {
                    detectionTime = millis();
                    incrementSection();
                    stopDrive();
                    show("STOP");
                    delay(2000);
                }
            }
        break;
      }

    }
}
