#include "ContinuousServo.h"

/**
 * ContinuousServo object, consists of a motor and a magnetic encoder
 */
ContinuousServo::ContinuousServo(int motorPin1, int motorPin2, 
int pwmChannel1, int pwmChannel2) {
    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2;
    this->pwmChannel1 = pwmChannel1;
    this->pwmChannel2 = pwmChannel2;
}

/**
 * Sets up the ContinuousServo
 */
void ContinuousServo::begin(MagneticEncoder* enc) {
    // Setup pwm channels
    // args: (pwmchannel to use,  frequency in Hz, number of bits)
    ledcSetup(pwmChannel1, 100, 12);
    ledcAttachPin(motorPin1, pwmChannel1);
    ledcSetup(pwmChannel2, 100, 12);
    ledcAttachPin(motorPin2, pwmChannel2);

    this->encoder = enc;
    this->targetAngle = this->encoder->getRelAngle();

    // PID controller object
    this->pidController = new PID(&this->Input, &this->Output, &this->Setpoint, 
                                this->Pk, this->Ik, this->Dk, DIRECT);
    this->pidController->SetMode(AUTOMATIC);              // PID Setup
    this->pidController->SetOutputLimits(-BIT_12_LIMIT, BIT_12_LIMIT);
    this->pidController->SetSampleTime(20);

}

/**
 * Drive the motor with a certain duty cycle. 
 * Positive sign means...
 * Negative sign means...
 * @param signedDuty 
 */
void ContinuousServo::drivePWM(int signedDuty) {
    int duty = (int) abs(signedDuty);
    duty = constrain(duty, 0, BIT_12_LIMIT);

    if (signedDuty <= 0) {
        ledcWrite(pwmChannel2, 0);
        ledcWrite(pwmChannel1, duty);
    } else {
        ledcWrite(pwmChannel1, 0);
        ledcWrite(pwmChannel2, duty);
    }
}

void ContinuousServo::moveBy(int degrees) {
    this->targetAngle = this->encoder->getRelAngle() + degrees;

}

void ContinuousServo::moveTo(int degrees) {
    this->targetAngle = degrees;
}

/**
 * PID Sequence uses relative encoder values
 */
void ContinuousServo::PIDSequence(int targetAngle) {
    float encAngle = this->encoder->readAngle(); // Increment the encoder angle
    double angle = this->encoder->getRelAngle();

    // PID Feedback
    float angleError = targetAngle - angle;
    //  this->encoder->angleDifference(targetValue, angle);
    Input = angleError;
    this->pidController->Compute();
    this->drivePWM(Output);

    Serial.print(angle, 2);
    Serial.println(" deg (relativeAngle)");
    Serial.print("Angle read: ");
    Serial.println(encAngle);
    Serial.print("Target angle: ");
    Serial.println(targetAngle);
    Serial.print("Angle error: ");
    Serial.println(angleError);
    Serial.print("PID Output: ");
    Serial.println(Output);
    Serial.println("-----");
}

void ContinuousServo::testSequence() {
    unsigned long startTime = millis();
    this->moveTo(60);
    while (millis() - startTime < 500000) {
        this->loop();
        delay(500);

    }
    this->moveTo(300);
    startTime = millis();
    while (millis() - startTime < 5000) {
        this->loop();
    }

    this->drivePWM(0);

    // this->moveBy(360);
    // startTime = millis();
    // while (millis() - startTime < 1000) {
    //     this->loop();
    // }
}

/**
 * Run this inside void loop to enable servo feedback
 */
void ContinuousServo::loop() {
    this->PIDSequence(this->targetAngle);
}




// PID Tuning
// int newKp = map(analogRead(P_Pin), 0, BIT_12_LIMIT, 0, 1000);
// double newKd = ((double) map(analogRead(D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;
// Serial.print("Kp: ");
// Serial.println(newKp);
// Serial.print("Kd: ");
// Serial.println(newKd);
// myPID.SetTunings(newKp, 0, newKd);