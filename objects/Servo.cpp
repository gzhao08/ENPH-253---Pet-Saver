#include "Servo.h"

/** 3% to 11% possible for the servo 
 * 11.9% duty cycle is claw fully open
 * 10.2% is horizontal
 * 9.4% is fully closed
 */

/**
 * Object representing a servo motor
 */
Servo::Servo(int motorPin, int pwmChannel) {
    this->motorPin = motorPin; 
    this->pwmChannel = pwmChannel;
}

/**
 * Sets up servo object 
 */
void Servo::begin() {
    ledcSetup(this->pwmChannel, this->servoFreq, 12); 
    ledcAttachPin(this->motorPin, this->pwmChannel);
}

/**
 * moves servo to given position
 * @param angle position in degrees to move servo to
 */
void Servo::writePosition(int angle) {
    // convert angle to PWM frequency out of 4096 (12 bits)
    int position = angle * 369 / 180 + 123;
    if (this->MIN_POSITION <= position && this->MAX_POSITION >= position) {
        ledcWrite(this->pwmChannel, position);
        this->positionDegrees = angle; 
        this->dutyCycle = int(angle * 12 / 180 + 3);

    }
}

/**
 * moves servo to position corresponding to a certain duty cycle
 * @param duty must be between 3 (3% = 0 deg) and 12 (12% = 180 deg)
 */
void Servo::writeDutyCycle(float duty) {
    if (this->MIN_PERCENT <= duty && this->MAX_PERCENT >= duty) {
        ledcWrite(this->pwmChannel, int(duty * 4096));
        this->dutyCycle = duty; 
        this->positionDegrees = (duty - 3) * 180 / 12;
    }
}
 
/**
 * @return servo position in degrees (0 to 180 CW from west to east) or -1 if servo has not been moved yet
 */
float Servo::readPosition() {
    return this->positionDegrees;
}

/**
 * @return servo duty cycle between 3 and 12 or -1 if servo has not been moved yet
 */
float Servo::readDutyCycle() {
    return this->dutyCycle; 
}

