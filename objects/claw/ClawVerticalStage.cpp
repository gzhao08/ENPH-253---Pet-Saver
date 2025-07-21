#include "ClawVerticalStage.h"

/**
 * ClawVerticalStage object, consists of a continuous servo and a microswitch
 */
ClawVerticalStage::ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin) : motor(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), mswitch(switchPin) {}


/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to vertical stage
 * @param mswitch microswitch object related to vertical stage
 */
void ClawVerticalStage::begin() {
    motor.begin();
    mswitch.begin();
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
            this->motor->home(); 
            isHome = true;
        }
    }  
}

float ClawVerticalStage::getPosition() {
    float angle = this->motor->getAngle(); 
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
        this->motor->moveBy(-1 * moveAngle); //negative is CW, CW is up
    }
}

void ClawVerticalStage::testSequence() {

}

