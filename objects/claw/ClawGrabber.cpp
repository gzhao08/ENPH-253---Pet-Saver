#include "ClawGrabber.h"

/**
 * ClawGrabber object, consists of a servo
 */
ClawGrabber::ClawGrabber(int motorPin, int pwmChannel) : motor(motorPin, pwmChannel) {}

/**
 * Grabber setup
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
    this->motor.writeDutyCycle(this->PARALLEL_DUTY);
}

void ClawGrabber::close() {
    this->motor.writePosition(this->CLOSE_DEG);
}

void ClawGrabber::open() {
    this->motor.writePosition(this->OPEN_DEG);
}

void ClawGrabber::parallel() {
    this->motor.writePosition(this->PARALLEL_DEG);
}

void ClawGrabber::testSequence() {
    this->close();

    delay(2000);

    this->parallel();

    delay(2000);

    this->open();

    delay(2000);
}
    

