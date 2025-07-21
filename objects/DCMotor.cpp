#include "DCMotor.h"

DCMotor::DCMotor(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int maxVoltage) {
    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2;
    this->pwmChannel1 = pwmChannel1;
    this->pwmChannel2 = pwmChannel2;
    this->maxVoltage = maxVoltage;
}

/**
 * Initialize the DCMotor
 * Sets up the PWM channels for the motor pins
 */
void DCMotor::begin() {
    ledcSetup(this->pwmChannel1, this->pwmFrequency, this->pwmResolution);
    ledcAttachPin(motorPin1, pwmChannel1);
    ledcSetup(this->pwmChannel2, this->pwmFrequency, this->pwmResolution);
    ledcAttachPin(motorPin2, pwmChannel2);

    this->maxDutyCycle = map(this->maxVoltage, 0, this->hBridgeVoltage, 0, BIT_12_LIMIT);
}

/**
 * Drive the motor with a certain duty cycle. 
 * @param signedDuty 
 * When looking at the side of the motor with the terminals, 
 *  Driving IN1 HIGH will rotate the motor clockwise.
 *  Driving IN2 HIGH will rotate the motor counter-clockwise.
 */
void DCMotor::drivePWM(int signedDuty) {
    int duty = abs(signedDuty);
    duty = constrain(duty, 0, this->maxDutyCycle);

    if (signedDuty > 0) {
        ledcWrite(pwmChannel2, 0);
        ledcWrite(pwmChannel1, duty);
    } else {
        ledcWrite(pwmChannel1, 0);
        ledcWrite(pwmChannel2, duty);
    }
}