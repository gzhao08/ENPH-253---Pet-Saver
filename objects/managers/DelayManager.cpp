#include "DelayManager.h"

DelayManager::DelayManager(unsigned long timeIntervalMs) {
    this->timeInterval = timeIntervalMs;
    this->lastTriggerTime = millis();
}

void DelayManager::setTimeInterval(unsigned long timeIntervalMs) {
    this->timeInterval = timeIntervalMs;
}

void DelayManager::reset() {
    this->lastTriggerTime = millis();
}

bool DelayManager::isElapsed() {
    return (millis() - this->lastTriggerTime) >= this->timeInterval;
}

bool DelayManager::checkAndReset() {
    if (isElapsed()) {
        reset();
        return true;
    }
    return false;
}