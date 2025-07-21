#include "ClawBase.h"

/**
 * ClawBase object, consists of a continuous servo and a microswitch
 */
ClawBase::ClawBase(){}

/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to claw base
 * @param mswitch microswitch object related to claw base
 */
void ClawBase::begin(ContinuousServo* motor, Microswitch* mswitch) {
    this->motor = motor;
    this->mswitch = mswitch;
}

void ClawBase::home() {
    bool home = false; 
    while (!home) {
        this->motor->moveBy(2);
        if (this->mswitch->isPressed()) {
            this->motor->home();
            home = true;
        }
    }    
}

float ClawBase::getPosition() {
    return -1 * this->motor->getAngle(); 
}

/**
 * rotates claw base to desired position (always rotates towards front of robot)
 */
void ClawBase::setPosition(float angle) {
    if (this->MIN_POSITION <= angle && this->MAX_POSITION >= angle) {
        float currentPos = this->getPosition();
        float moveAngle = -1 * (currentPos + 1000) - (angle + 1000);
        this->motor->moveBy(moveAngle); //CCW is positive so multiply by -1
    }
}

void ClawBase::testSequence() {

}

