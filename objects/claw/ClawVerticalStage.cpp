#include "ClawVerticalStage.h"

/**
 * ClawVerticalStage object, consists of a continuous servo and a microswitch
 */
ClawVerticalStage::ClawVerticalStage(){}

/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to vertical stage
 * @param mswitch microswitch object related to vertical stage
 */
void ClawVerticalStage::begin(ContinuousServo* motor, Microswitch* mswitch) {
    this->motor = motor;
    this->mswitch = mswitch;
}
/**
 * Home the claw vertical stage by moving it until the microswitch is pressed
 * This will set the home position of the claw vertical stage
 */
void ClawVerticalStage::home() {
    bool isHome = false; 
    while (!isHome) {
        this->motor->moveBy(180);
        if (this->mswitch->isPressed()) {
            // this->motor-> // TODO: set homing pos
            isHome = true;
        }
    }  
}

float ClawVerticalStage::getPosition() {
    //this->motor-> //TODO: add methods to continuous servo to get positional information

}

/**
 * raises claw to desired height
 * @param height above minimum height in mm
 */
void ClawVerticalStage::setPosition(float height) {
    if (this->MIN_HEIGHT <= height && this->MAX_HEIGHT >= height) {
        currentPos = this->getPosition();
        moveAngle = (currentPos - height) * 360 / 2; // 2mm corresponds to 1 rotation
        this->motor->moveBy(moveAngle); //TODO: check -- assumed CW is positive; check if CW is up or down
    }
}

void ClawVerticalStage::testSequence() {

}

