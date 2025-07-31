#include "SectionManager.h"
#include "../GlobalConstants.h"

#define CONSECUTIVE_THRESH 3 // number of consecutive measurements to consider a section change

SectionManager::SectionManager(boolean useDisplay) {
    currentSection = 0; // Start at section 0
    objectCount = 0; // Initialize object count
    rightLidar = Adafruit_VL53L0X();
    leftLidar = Adafruit_VL53L0X();
    if (useDisplay) {
        display = Adafruit_SSD1306(128, 64, &Wire, -1);
    }
}



boolean SectionManager::detectOutOfRange(int distance) {
    if (distance >= 600) { // assuming 8191 is the out-of-range value for the sensor
        return true;
    }

    return false;
}

boolean SectionManager::detect(int distance, int threshold, bool senseDir, int consecutiveCount) {
    
    if (senseDir) {
        if (distance <= threshold) {
            numConsecutive++;
            if (numConsecutive >= consecutiveCount) {
                numConsecutive = 0; // reset after detection
                currentSection++;
                return true;
            }
        }
        else {
            numConsecutive = 0; // reset if not detecting
        }
        return false; 
    }

    else {
        if (distance >= threshold) {
            numConsecutive++;
            if (numConsecutive >= consecutiveCount) {
                numConsecutive = 0; // reset after detection
                currentSection++;
                return true;
            }
        }
        else {
            numConsecutive = 0; // reset if not detecting
        }
        return false; 
    }
}
