#include "Microswitch.h"

/**
 * Object representing a microswitch
 */
Microswitch::Microswitch(int switchPin, bool normallyOpen) {
    this->switchPin = switchPin; 
    this->normallyOpen = normallyOpen; 
}

/**
 * sets up microswitch object
 */
void Microswitch::begin() {
    pinMode(this->switchPin, INPUT);
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



