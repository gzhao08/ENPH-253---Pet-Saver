#include "ClawGrabber.h"

/**
 * ClawGrabber object, consists of a servo
 */
ClawGrabber::ClawGrabber(int motorPin, int pwmChannel) : motor(motorPin, pwmChannel) {}

/**
 * Sets up magnetic encoder
 * @param motor servo object related to claw grabber
 */
void ClawGrabber::begin() {
    this->motor.begin();
}

/**
 * @return servo position in degrees (0 to 180 CW from west to east) or -1 if servo has not been moved yet
 */
float ClawGrabber::getPositionDegrees() {
    return this->motor.readPosition(); 
}

/**
 * @return servo duty cycle between 3 and 12 or -1 if servo has not been moved yet
 */
float ClawGrabber::getPositionDutyCycle() {
    return this->motor.readDutyCycle(); 
}

/**
 * moves servo to given position
 * @param angle position in degrees to move servo to
 */
void ClawGrabber::setPositionDegrees(float angle) {
    this->motor.writePosition(angle);
}

/**
 * moves servo to position corresponding to a certain duty cycle
 * @param duty must be between 3 (3% = 0 deg) and 12 (12% = 180 deg)
 */
void ClawGrabber::setPositionDutyCycle(float cycle) {
    this->motor.writeDutyCycle(cycle);
}

/**
 * sets claw arms to be parallel to one another
 */
void ClawGrabber::setParallel() {
    this->motor.writeDutyCycle(this->PARALLEL);
}

void ClawGrabber::testSequence() {

}
    

