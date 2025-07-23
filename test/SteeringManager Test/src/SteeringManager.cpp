#include "SteeringManager.h"

volatile boolean drive = false; // boolean indicating when to stop driving ; should be global and changed via interrupts

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
    drive = true;
    while (drive) {
        leftMotor->drivePWM(duty);
        rightMotor->drivePWM(duty);
    }
    this->stop();
}

void SteeringManager::backward(int duty) {
    drive = true;
    while (drive) {
        leftMotor->drivePWM(-duty);
        rightMotor->drivePWM(-duty);
    }
    this->stop();
}

void SteeringManager::stop() {
    leftMotor->stop();
    rightMotor->stop();
}

void SteeringManager::lineFollow(int baseSpeed) {
    drive = true;
    this->array.takeReading(false);
    input = this->array.getError();
    unsigned long lastComputeTime = millis();
    this->array.update();
    while (drive) {
        // only poll and calculate PID at PID sample rate
        if (millis() - lastComputeTime >= this->PIDSampleTime) {
            // compute PID
            pidController.Compute();
            lastComputeTime = millis();
            // drive motors
            leftMotor->drivePWM(baseSpeed-output);
            rightMotor->drivePWM(baseSpeed+output);
            
        }
        // update IR data every cycle so that error is accurate
        this->array.takeReading(false);
        input = this->array.getError();
        this->array.update();
    }
    this->stop();
}