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
    display(128, 64, &Wire1, -1) {}

void SectionManager::begin(boolean usingDisplay) {

    Serial.println("Initializing display");

    if (usingDisplay) {
        this->useDisplay = true;
        
        byte error = -1;
        while (error != 0) {
            Wire1.beginTransmission(0x3C);
            error = Wire1.endTransmission();
            delay(500);
        }

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
        Serial.println("Succesfully initialized display");
    }

    

    rightLidar.setBus(&Wire);
    leftLidar.setBus(&Wire1);

    Serial.println("Initializing Right VL53L0X...");
    while (!rightLidar.init(false)) {
        Serial.println("Failed to boot Right VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Right VL53L0X Initialized!");

    Serial.println("Initializing Left VL53L0X...");
    while (!leftLidar.init(false)) {
        Serial.println("Failed to boot Left VL53L0X, retrying...");
        delay(100);
    }

    Serial.println("Left VL53L0X Initialized!");

    Serial.println("Both VL53L0X ready!");

    Serial.printf("SectionManager -- drive : %p\n", &drive);

    leftLidar.startContinuous();
    rightLidar.startContinuous();
    

    if (useDisplay) {
        display.clearDisplay();
        display.setTextSize(3);
        display.setCursor(0, 0);
        display.println("VL53L0X");
        display.println("Ready!");
        display.display();
        display.clearDisplay();
        delay(1000);
        display.setCursor(0, 24);
        display.setTextSize(2);
        display.println("SWIPE RIGHT");
        display.display();
        display.clearDisplay();
        delay(500);
    }
}

boolean SectionManager::detectOutOfRange(bool useRight) {
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    if (distance >= 500) {
        return true;
    }
    return false;
}

int SectionManager::getMeasurement(bool useRight) {
    return (useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters());
}

boolean SectionManager::detectCloser(bool useRight, int threshold, int consecutiveCount) {
    
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    
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
    int distance = useRight ? rightLidar.readRangeContinuousMillimeters() : leftLidar.readRangeContinuousMillimeters();
    
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
                Serial.println("found doorway");
                incrementSection();
                if (useDisplay) {
                    display.clearDisplay();
                    display.setCursor(30, 24);
                    display.setTextSize(2);
                    display.println("PET 1");
                    display.display();
                }
                recordStartTime();
                currentSpeed = 800;
            }
            break;
        }

        case SectionManager::PET_1: {
            if (millis() - startMovementTime > 1000) {
                if (detectCloser(true, 375, 1)) {
                    Serial.println("found pet 1");
                    incrementSection();
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(30, 24);
                        display.setTextSize(2);
                        display.println("RAMP");
                        display.display();
                    }
                    recordStartTime();
                    robotState = RobotState::STOPPED;
                    currentSpeed = 1100;
                }
            }
            
            break;
        }

      case SectionManager::RAMP: {
        if (millis() - startMovementTime > 500) {
                if (detectCloser(true, 350, 3)) {
                    Serial.println("found ramp");
                    incrementSection();
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(30, 24);
                        display.setTextSize(2);
                        display.println("RAMP END");
                        display.display();
                    }
                    recordStartTime();
                    currentSpeed = 1800;
                }
            }
        break;
      }

      case SectionManager::RAMP_END: {
        if (millis() - startMovementTime > 2000) {
                if (detectCloser(false, 350, 3)) {
                    Serial.println("found ramp end");
                    incrementSection();
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(30, 24);
                        display.setTextSize(2);
                        display.println("PET_3");
                        display.display();
                    }
                    recordStartTime();
                    startLineFollow();
                    currentSpeed = 800;
                }
            }
        break;
      }

      case SectionManager::PET_3: {
        if (millis() - startMovementTime >= 2500) {
            if (detectCloser(false, 300, 2)) {
                Serial.println("found pet 3");
                incrementSection();
                if (useDisplay) {
                    display.clearDisplay();
                    display.setCursor(30, 24);
                    display.setTextSize(2);
                    display.println("RAMP_B");
                    display.display();
                }
                recordStartTime();
                stopDrive();
                currentSpeed = 900;
            }
        }
            
        break;
      }

      case SectionManager::RAMP_END_BACK: {
            // unsigned int timePassed = millis() - startMovementTime;
            // Serial.printf("PET_4 time: %d\n", timePassed);
            if (millis() - startMovementTime >= 1000) {
                if (detectFurther(true, 350, 3)) {
                    Serial.println("found ramp");
                    incrementSection();
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(0, 24);
                        display.setTextSize(2);
                        display.println("WINDOW_B");
                        display.display();
                    }
                    recordStartTime();
                    stopDrive();
                    currentSpeed = 1000;
                }
            }
            
            break;
      }

      case SectionManager::WINDOW_BACKWARD: {
            
            if (detectCloser(true, 350, 2)) {
                incrementSection();
                if (useDisplay) {
                    display.clearDisplay();
                    display.setCursor(0, 24);
                    display.setTextSize(2);
                    display.println("PET_5");
                    display.display();
                }
                recordStartTime();
                stopDrive();
                currentSpeed = 1200;
            }
            
            
            
            break;
    }

    // case SectionManager::WINDOW_FORWARD: {
            
    //     if (detectCloser(true, 300, 2)) {
    //             incrementSection();
    //             if (useDisplay) {
    //                 display.clearDisplay();
    //                 display.setCursor(30, 24);
    //                 display.setTextSize(2);
    //                 display.println("PET_5");
    //                 display.display();
    //             }
    //             recordStartTime();
    //             stopDrive();
    //             currentSpeed = 1100;
    //     }
            
    //     break;
    // }

      case SectionManager::PET_5: {
            if (millis() - startMovementTime >= 2500) {
                if (detectCloser(false, 400, 2)) {
                    currentSection = RAMP_END_BACK_2;
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(15, 24);
                        display.setTextSize(2);
                        display.println("RAMP_B2");
                        display.display();
                    }
                    recordStartTime();
                    stopDrive();
                    currentSpeed = 900;
                }
            }
            
            break;
      }
      case SectionManager::RAMP_END_BACK_2: {
            // unsigned int timePassed = millis() - startMovementTime;
            // Serial.printf("PET_4 time: %d\n", timePassed);
            if (millis() - startMovementTime >= 3000) {
                if (detectFurther(true, 350, 3)) {
                    Serial.println("found ramp");
                    incrementSection();
                    if (useDisplay) {
                        display.clearDisplay();
                        display.setCursor(0, 24);
                        display.setTextSize(2);
                        display.println("DONE");
                        display.display();
                    }
                    recordStartTime();
                    stopDrive();
                    //currentSpeed = 1000;
                }
            }
            
            break;
        }
      }

    //   case SectionManager::PET_6: {
    //         if (millis() - startMovementTime >= 2000) {
    //             if (detectCloser(true, 350, 2)) {
    //                 currentSection = RAMP_END_BACK_3;
    //                 if (useDisplay) {
    //                     display.clearDisplay();
    //                     display.setCursor(30, 24);
    //                     display.setTextSize(2);
    //                     display.println("DONE");
    //                     display.display();
    //                 }
    //                 recordStartTime();
    //                 stopDrive();
    //                 currentSpeed = 1000;
    //             }
    //         }
            
    //         break;
    //   }
    
}

void SectionManager::setDetectionTime() {
    detectionTime = millis();
}

