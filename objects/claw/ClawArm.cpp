//151mm (~6") per rotation of horizontal gear (d = 48mm)

#include "ClawArm.h"

/**
 * ClawArm object, consists of a continuous servo and a microswitch
 */
ClawArm::ClawArm(){}

/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to claw arm
 * @param mswitch microswitch object related to claw arm
 */
void ClawArm::begin(ContinuousServo* motor, Microswitch* mswitch) {
    this->motor = motor;
    this->mswitch = mswitch;
}

void ClawArm::home() {
bool home = false; 
    while (!home) {
        this->motor->moveBy(10);
        if (this->mswitch->isPressed()) {
            this->motor-> // TODO: set homing pos
            home = true;
        }
    }
}

float ClawArm::getPosition() {
    this->motor-> //TODO: add methods to continuous servo to get positional information
}

/**
 * @param position final position of horizontal stage in mm from home position (0)
 */
void ClawArm::setPosition(float position) {
    if (this->MIN_POSITION <= angle && this->MAX_POSITION >= angle) {
        currentPos = this->getPosition();
        moveAngle = (currentPos - position) * 360 / 151;
        this->motor->moveBy(moveAngle); //TODO: check -- assumed CW is positive
    }
}

void ClawArm::testSequence() {

}

