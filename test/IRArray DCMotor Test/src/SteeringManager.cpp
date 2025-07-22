#include "SteeringManager.h"

SteeringManager::SteeringManager(DCMotor* left, DCMotor* right)
    : leftMotor(left), rightMotor(right),
      pidController(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT)
{

}

void SteeringManager::begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin) {
    // PID
    this->pidController.SetMode(AUTOMATIC);
    this->pidController.SetOutputLimits(-leftMotor->getMaxDutyCycle(), leftMotor->getMaxDutyCycle()); // Set output limits to motor max duty cycle
    this->pidController.SetSampleTime(this->PIDSampleTime);

    // Motor
    leftMotor->begin();
    rightMotor->begin();

    // IR
    this->array.begin(outerLeftPin, innerLeftPin, innerRightPin, outerRightPin);
}


void SteeringManager::forward(int duty) {
    leftMotor->drivePWM(duty);
    rightMotor->drivePWM(duty);
}

void SteeringManager::backward(int duty) {
    leftMotor->drivePWM(-duty);
    rightMotor->drivePWM(-duty);
}

void SteeringManager::lineFollow(int baseSpeed) {
    followLine = true;
    this->array.takeReading(false);
    input = this->array.getError();
    unsigned long lastComputeTime = millis();
    this->array.update();
    while (followLine) {
        if (millis() - lastComputeTime >= this->PIDSampleTime) {
            // compute PID
            pidController.Compute();
            lastComputeTime = millis();
            // drive motors
            leftMotor->drivePWM(baseSpeed-output);
            rightMotor->drivePWM(baseSpeed+output);
        }
        this->array.update();
        this->array.takeReading(false);
        input = this->array.getError();
    }
}