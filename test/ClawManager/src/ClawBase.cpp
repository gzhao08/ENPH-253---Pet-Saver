#include "ClawBase.h"

/**
 * ClawBase object, consists of a continuous servo 
 */
ClawBase::ClawBase(){}

/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to claw base
 */
void ClawBase::begin(ContinuousServo* motor) {
    this->motor = motor;
}

void ClawBase::home() {
    //put homing here    
}

float ClawBase::getPosition() {
    this->motor-> //add methods to continuous servo to get positional information
}

/**
 * rotates claw base to desired position (always rotates towards front of robot)
 */
void ClawBase::setPosition(float angle) {
    if (-120 <= angle && 120 >= angle) {
        currentPos = this->getPosition();
        if (angle > currentPos) {
            moveAngle = 
        }
        else {

        }
        this->motor->moveBy(moveAngle); 
    }
    else {
        return -1;
    }
}

void ClawBase::testSequence() {

}

