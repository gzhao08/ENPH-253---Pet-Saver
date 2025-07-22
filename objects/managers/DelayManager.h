#pragma once

#include <Arduino.h>
// Local Libraries:

class DelayManager {
    private:
        unsigned long lastTriggerTime;
        unsigned long timeInterval; // milliseconds
    public:
        DelayManager(unsigned long timeIntervalMs);
        void start();
        void setTimeInterval(unsigned long timeIntervalMs);
        void reset();
        bool isElapsed();
        bool checkAndReset();        
};