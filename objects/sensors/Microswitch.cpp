#include "Microswitch.h"

/**
 * Object representing a microswitch
 */
Microswitch::Microswitch(int pin, bool normallyOpen) {
    this->switchPin = switchPin; 
    this->normallyOpen = normallyOpen; 
}

/**
 * sets up microswitch object
 */
void Microswitch::begin() {
    pinMode(this->switchPin, INPUT_PULLUP);
}

/**
 * @return true if switch is pressed, false otherwise
 */
bool Microswitch::isPressed() {
    if (this->normallyOpen) {
        return digitalRead(this->switchPin);
    }
    else {
        return !digitalRead(this->switchPin);
    }
} 

/**
 * 
 */
//bool justReleased() {} //TODO: TIMEFRAME? How are we storing values?

/**
 * 
 */
//bool justPressed() {} //TODO: TIMEFRAME?




