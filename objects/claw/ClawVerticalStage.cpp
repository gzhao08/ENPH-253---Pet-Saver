#include "ClawVerticalStage.h"

/**
 * ClawVerticalStage object, consists of a continuous servo and a microswitch
 */
ClawVerticalStage::ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
motorVertical(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
mswitchVertical(switchPin, normallyOpen) {}


/**
 * Sets up magnetic encoder
 * @param motorVertical Continuous servo object related to vertical stage
 * @param mswitchVertical microswitch object related to vertical stage
 */
void ClawVerticalStage::begin(WireManager* wireManager) {
    motorVertical.begin(wireManager);
    mswitchVertical.begin();
}

/**
 * Home the claw vertical stage by moving it until the microswitch is pressed
 * This will set the home position of the claw vertical stage
 */
void ClawVerticalStage::home() {
    bool isHome = false; 
    while (!isHome) {
        this->motorVertical.moveBy(180);
        if (this->mswitchVertical.isPressed()) {
            this->motorVertical.home(); 
            isHome = true;
        }
    }  
}

float ClawVerticalStage::getPosition() {
    float angle = this->motorVertical.getAngle(); 
    return angle * 2 / 360;

}

/**
 * raises claw to desired height
 * @param height above minimum height in mm
 */
void ClawVerticalStage::setPosition(float height) {
    //CW moves mechanism up
    if (this->MIN_HEIGHT <= height && this->MAX_HEIGHT >= height) {
        float currentPos = this->getPosition();
        float moveAngle = (height - currentPos) * 360 / 2; // 2mm corresponds to 1 rotation, positive if need to move up
        this->motorVertical.moveBy(-1 * moveAngle); //negative is CW, CW is up
    }
}

void ClawVerticalStage::testSequence() {

}

