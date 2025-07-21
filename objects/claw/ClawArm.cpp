//151mm (~6") per rotation of horizontal gear (d = 48mm)

#include "ClawArm.h"

/**
 * ClawArm object, consists of a continuous servo and a microswitch
 */
ClawArm::ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin) : 
motor(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), mswitch(switchPin) {}

/**
 * Sets up magnetic encoder
 * @param motor Continuous servo object related to claw arm
 * @param mswitch microswitch object related to claw arm
 */
void ClawArm::begin() {
    this->motor.begin();
    this->mswitch.begin();
}

void ClawArm::home() {
bool home = false; 
    while (!home) {
        this->motor.moveBy(10);
        if (this->mswitch.isPressed()) {
            this->motor.home();
            home = true;
        }
    }
}

float ClawArm::getPosition() {
    float angle = this->motor.getAngle(); 
    return angle * 151 / 360;
}

/**
 * @param position final position of horizontal stage in mm from home position (0)
 */
void ClawArm::setPosition(float position) {
    if (this->MIN <= position && this->MAX >= position) {
        float currentPos = this.getPosition();
        float moveAngle = -1 * (currentPos - position) * 360 / 151;
        this->motor.moveBy(moveAngle); //CCW is positive so multiply by -1
    }
}

void ClawArm::testSequence() {

}

