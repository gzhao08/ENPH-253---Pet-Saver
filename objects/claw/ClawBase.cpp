#include "ClawBase.h"

/**
 * ClawBase object, consists of a continuous servo and a microswitch
 */
ClawBase::ClawBase(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
motorBase(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
mswitchBase(switchPin, normallyOpen) {}


/**
 * Sets up magnetic encoder
 * @param motorBase Continuous servo object related to claw base
 * @param mswitchBase microswitch object related to claw base
 */
void ClawBase::begin(WireManager* wireManager) {
    this->motorBase.begin(wireManager);
    this->mswitchBase.begin();
}

void ClawBase::home() {
    bool home = false; 
    while (!home) {
        this->motorBase.moveBy(2);
        if (this->mswitchBase.isPressed()) {
            this->motorBase.home();
            home = true;
        }
    }    
}

float ClawBase::getPosition() {
    return -1 * this->motorBase.getAngle(); 
}

/**
 * rotates claw base to desired position (always rotates towards front of robot)
 */
void ClawBase::setPosition(float angle) {
    if (this->MIN_POSITION <= angle && this->MAX_POSITION >= angle) {
        float currentPos = this->getPosition();
        float moveAngle = -1 * (currentPos + 1000) - (angle + 1000);
        this->motorBase.moveBy(moveAngle); //CCW is positive so multiply by -1
    }
}

void ClawBase::testSequence() {

}

