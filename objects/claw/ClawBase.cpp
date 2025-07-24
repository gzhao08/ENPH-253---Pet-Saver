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

void ClawBase::setAsHome() {
    this->motorBase.setAsHome(); // Set home position to minimum angle
}

void ClawBase::homingSequence() {
    int overshootMotorAngle = (this->MIN_BASE_ANGLE_POSITION - this->MAX_BASE_ANGLE_POSITION) * this->BASE_TO_ENCODER_ANGLE_CONVERSION * 1.1;
    this->motorBase.moveBy(overshootMotorAngle); 
    
    while (!this->mswitchBase.isPressed()) {
        this->motorBase.loop();
    }
    this->setAsHome();
    Serial.println("Base Homing sequence done");
}

/**
 * @return true if the claw base has reached the target position, false otherwise
 */
float ClawBase::reachedTarget() {
    return this->motorBase.reachedTarget();
}

float ClawBase::getPosition() {
    float encoderAngle = this->motorBase.getAngle();
    return (encoderAngle * this->ENCODER_TO_BASE_ANGLE_CONVERSION) + this->MIN_BASE_ANGLE_POSITION; // convert to mm
}

/**
 * rotates claw base to desired position (always rotates towards front of robot)
 */
void ClawBase::setPosition(float position) {
    float constrainedPosition = constrain(position, this->MIN_BASE_ANGLE_POSITION, this->MAX_BASE_ANGLE_POSITION);
    float shiftedPosition = constrainedPosition - this->MIN_BASE_ANGLE_POSITION;
    float relAngle = shiftedPosition * this->BASE_TO_ENCODER_ANGLE_CONVERSION;
    this->motorBase.moveTo(relAngle);
}

void ClawBase::loop() {
    this->motorBase.loop();
}

void ClawBase::testSequence() {

}

