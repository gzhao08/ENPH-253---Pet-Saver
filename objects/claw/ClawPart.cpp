#include "ClawPart.h"

/**
 * A claw part, consisting of a continuous servo and a microswitch
 */
ClawPart::ClawPart(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
continuousServo(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
microswitch(switchPin, normallyOpen) {}

/**
 * Sets up claw part
 */
void ClawPart::begin(WireManager* wireManager) {
    continuousServo.begin(wireManager);

    continuousServo.setPDTuning(this->Pk, this->Dk);
    continuousServo.setMaxVoltage(this->servoMaxVoltage); 
    continuousServo.tolerance = this->servoTolerance;

    microswitch.begin();
}

/**
 * Sets current position as home
 */

void ClawPart::setAsHome() {
    this->continuousServo.setAsHome();
}

/**
 * Home the claw by moving it until the microswitch is pressed
 * This will set the home position of the claw part
 */
void ClawPart::homingSequence() {
    int overshootAngle = -(this->ABS_POS_LIMIT - this->MIN_POSITION) * POS_TO_ENCODER_CONVERSION * 1.2;
    this->continuousServo.moveBy(overshootAngle); // Move down to ensure we are not at the top
    
    DelayManager timeout(10000);
    timeout.reset();
    while (!this->microswitch.isPressed()) {
        this->loop();
        if (timeout.checkAndReset()) {
            Serial.println("Homing timed out (10 seconds) for " + this->partName);
            break;
        }
    }  

    this->setAsHome();
    Serial.println("Current position of " + this->partName + " is set as home");
}

/**
 * @return true if the claw part has reached the target position, false otherwise
 */
bool ClawPart::reachedTarget() {
    return this->continuousServo.reachedTarget();
}

/**
 * Get position of claw part
 */
 float ClawPart::getPosition() {
    float angle = this->continuousServo.getAngle();
    return (angle * this->ENCODER_TO_POS_CONVERSION) + this->MIN_POSITION;
 }

 /** 
  * Move the claw part to the desired position
 */
void ClawPart::setPosition(float position) {
    float constrainedPosition = constrain(position, this->MIN_POSITION, this->MAX_POSITION);
    float shiftedPosition = constrainedPosition - this->MIN_POSITION;
    float relAngle = shiftedPosition * this->POS_TO_ENCODER_CONVERSION;
    this->continuousServo.moveTo(relAngle);
}

/**
 * Set PID tuning mode to true or false
 */
void ClawPart::setPIDTuningMode(bool mode) {
    this->continuousServo.setPIDTuningMode(mode);
}

/**
 * Set P and D tuning pin
 */
void ClawPart::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->continuousServo.setPIDTuningPins(P_Pin, D_Pin);
}

/**
 * Call this in void loop to enable claw actuation
 */
void ClawPart::loop() {
    this->continuousServo.loop();
}