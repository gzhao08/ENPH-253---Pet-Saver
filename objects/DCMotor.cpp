#include "DCMotor.h"

DCMotor::DCMotor(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int maxVoltage) {
    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2;
    this->pwmChannel1 = pwmChannel1;
    this->pwmChannel2 = pwmChannel2;
    this->maxVoltage = maxVoltage;
}

void DCMotor::begin() {
    ledcSetup(pwmChannel1, 5000, 8);
    ledcAttachPin(motorPin1, pwmChannel1);
    ledcSetup(pwmChannel2, 5000, 8);
    ledcAttachPin(motorPin2, pwmChannel2);
}