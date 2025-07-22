#include "SteeringManager.h"

SteeringManager::SteeringManager(DCMotor* left, DCMotor* right) {
    leftMotor = *left;
    rightMotor = *right;

    pidController(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
}

SteeringManager::begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin) {
    // PID
    this->pidController.SetMode(AUTOMATIC);
    this->pidController.SetOutputLimits(-this->leftMotor.getMaxDutyCycle(), this->leftMotor.getMaxDutyCycle()); // Set output limits to motor max duty cycle
    this->pidController.SetSampleTime(this->PIDSampleTime);



    // Motor
    this->leftMotor.begin();
    this->rightMotor.begin();

    // IR
    this->array.begin(outerLeftPin, innerLeftPin, innerRightPin, outerRightPin);
}


SteeringManager::forward(int duty) {
    this->leftMotor.drivePWM(duty);
    this->rightMotor.drivePWM(duty);
}

SteeringManager::backward(int duty) {
    this->leftMotor.drivePWM(-duty);
    this->rightMotor.drivePWM(-duty);
}

SteeringManager::lineFollow(int baseSpeed) {
    lineFollow = true;
    this->array.takeReading(false);
    input = this->array.getError();
    unsigned long lastComputeTime = millis();
    this->array.update();
    while (lineFollow) {
        if (millis() - lastComputeTime >= this->PIDSampleTime) {
            // compute PID
            pidController.compute();
            lastComputeTime = millis();
            // drive motors
            this->leftMotor.drivePWM(baseSpeed-output);
            this->rightMotor.drivePWM(baseSpeed+output);
        }
        this->array.update();
        this->array.takeReading(false);
        input = this->array.getError();
    }
}