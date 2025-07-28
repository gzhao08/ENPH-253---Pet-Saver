#include "Servo.h"

/** 3% to 11% possible for the servo 
 * 11.9% duty cycle is claw fully open
 * 10.2% is horizontal
 * 9.4% is fully closed
 */

/**
 * Object representing a servo motor
 */
MyServo::MyServo(int motorPin, int pwmChannel) {
    this->motorPin = motorPin; 
    this->pwmChannel = pwmChannel;
}

/**
 * Sets up servo object 
 */
void MyServo::begin() {
    ledcSetup(this->pwmChannel, this->servoFreq, 12); 
    ledcAttachPin(this->motorPin, this->pwmChannel);
}

/**
 * moves servo to given position
 * @param angle position in degrees to move servo to
 */
void MyServo::writePosition(int angle) {
    // convert angle to PWM frequency out of 4096 (12 bits)
    int position = angle * this->RANGE_POS / this->RANGE_DEG + this->MIN_POSITION_PWM;
    Serial.println("Angle: " + String(angle));
    Serial.println("Position: " + String(position));
    if (this->MIN_POSITION_PWM <= position && this->MAX_POSITION_PWM >= position) {
        ledcWrite(this->pwmChannel, position);
        this->positionDegrees = angle; 
        this->dutyCycle = int(angle * this->RANGE_DUTY / this->RANGE_DEG + this->MIN_POSITION_PWM);
    }
}

/**
 * moves servo to position corresponding to a certain duty cycle
 * @param duty must be between 3 (3% = 0 deg) and 12 (12% = 180 deg)
 */
void MyServo::writeDutyCycle(float duty) {
    if (this->MIN_PERCENT <= duty && this->MAX_PERCENT >= duty) {
        ledcWrite(this->pwmChannel, int(duty * 4096 / 100));
        this->dutyCycle = duty; 
        this->positionDegrees = (duty - this->MIN_PERCENT) * this->RANGE_DEG / this->RANGE_DUTY;
    }
}
 
/**
 * @return servo position in degrees (0 to 180 CW from west to east) or -1 if servo has not been moved yet
 */
float MyServo::readPosition() {
    return this->positionDegrees;
}

/**
 * @return servo duty cycle between 3 and 12 or -1 if servo has not been moved yet
 */
float MyServo::readDutyCycle() {
    return this->dutyCycle; 
}

