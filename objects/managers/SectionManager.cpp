#include "SectionManager.h"
#include "../GlobalConstants.h"

#define CONSECUTIVE_THRESH 7 // number of consecutive measurements to consider a section change

SectionManager::SectionManager() : currentSection(0), numConsecutive(0) {}

boolean SectionManager::detectDoorway(int distance) {
    if (distance < DOORWAY_THRESH) {
        numConsecutive++;
        if (numConsecutive >= CONSECUTIVE_THRESH) { // require 10 consecutive measurements to be a doorway
            currentSection = 1; // set to doorway section
            numConsecutive = 0;
            return true;
        }
    } else {
        numConsecutive = 0; // reset if not in doorway
    }
    return false;
}

boolean SectionManager::detectFirstPet(int distance) {
    if (distance < FIRST_PET_THRESH) {
        numConsecutive++;
        if (numConsecutive >= CONSECUTIVE_THRESH) { // require 10 consecutive measurements to detect first pet
            currentSection = 2; // set to first pet section
            numConsecutive = 0;
            return true;
        }
    } else {
        numConsecutive = 0; // reset if not detecting first pet
    }
    return false;
}

boolean SectionManager::detectRamp(int distance) {
    if (distance < RAMP_THRESH) {
        numConsecutive++;
        if (numConsecutive >= CONSECUTIVE_THRESH) { // require 10 consecutive measurements to detect ramp
            currentSection = 3; // set to ramp section
            numConsecutive = 0;
            return true;
        }
    } else {
        numConsecutive = 0; // reset if not detecting ramp
    }
    return false;
}

boolean SectionManager::detectEndOfRamp(int distance) {
    if (distance > RAMP_THRESH) {
        numConsecutive++;
        if (numConsecutive >= CONSECUTIVE_THRESH) { // require 10 consecutive measurements to detect end of ramp
            currentSection = 0; // reset to initial section
            numConsecutive = 0;
            return true;
        }
    } else {
        numConsecutive = 0; // reset if not detecting end of ramp
    }
    return false;
}

boolean SectionManager::detectOutOfRange(int distance) {
    if (distance >= 8190) { // assuming 8191 is the out-of-range value for the sensor
        return true;
    }

    return false;
}
