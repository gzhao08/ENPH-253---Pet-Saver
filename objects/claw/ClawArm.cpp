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
            this->motor->home(); // TODO: set homing pos
            home = true;
        }
    }
}

float ClawArm::getPosition() {
    float angle = this->motor->getAngle(); 
    return angle * 151 / 360;
}

/**
 * @param position final position of horizontal stage in mm from home position (0)
 */
void ClawArm::setPosition(float position) {
    if (this->MIN <= position && this->MAX >= position) {
        float currentPos = this->getPosition();
        float moveAngle = -1 * (currentPos - position) * 360 / 151;
        this->motor->moveBy(moveAngle); //CCW is positive so multiply by -1
    }
}

void ClawArm::testSequence() {

}

