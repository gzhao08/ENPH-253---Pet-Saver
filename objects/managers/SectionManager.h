#pragma once
#include <Arduino.h>

class SectionManager {
    private:
        int currentSection; // 0, 1, or 2
        int numConsecutive;

    public:
        SectionManager();

        int getCurrentSection() {
            return currentSection;
        }

        boolean detectDoorway(int distance);
        boolean detectFirstPet(int distance);
        boolean detectRamp(int distance);
        boolean detectEndOfRamp(int distance);
        boolean detectOutOfRange(int distance);
        boolean detect(int distance, int threshold, bool senseDir);
};



