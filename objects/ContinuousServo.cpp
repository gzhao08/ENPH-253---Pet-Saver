#include "ContinuousServo.h"

unsigned long lastPrint = 0;
/**
 * ContinuousServo object, consists of a motor and a magnetic encoder
 * This is a RELATIVE ANGLE based application (not the absolute reading of the encoder)
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

    // int newKp = map(analogRead(P_Pin), 0, BIT_12_LIMIT, 0, 1000);
// double newKd = ((double) map(analogRead(D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;
// Serial.print("Kp: ");
// Serial.println(newKp);
// Serial.print("Kd: ");
// Serial.println(newKd);
// myPID.SetTunings(newKp, 0, newKd);

    // Pk =  map(analogRead(34), 0, BIT_12_LIMIT, 0, 1000);
    // Serial.print("Kp: ");
    // Serial.println(Pk);

    // Dk =  map(analogRead(38), 0, BIT_12_LIMIT, 0, 1000)/30;
    // Serial.print("Kd: ");
    // Serial.println(Dk);

    // PID controller object
    this->pidController = new PID(&this->Input, &this->Output, &this->Setpoint, 
                                this->Pk, this->Ik, this->Dk, DIRECT);
    this->pidController->SetMode(AUTOMATIC);              // PID Setup
    this->pidController->SetOutputLimits(-BIT_12_LIMIT, BIT_12_LIMIT);
    this->pidController->SetSampleTime(this->PIDSampleTime);

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

void ContinuousServo::moveBy(float degrees) {
    this->stableCounter = 0;
    this->targetAngle = this->encoder->getRelAngle() + degrees;

}

void ContinuousServo::moveTo(float degrees) {
    this->stableCounter = 0;
    this->targetAngle = degrees;
}

/**
 * PID Sequence uses relative encoder values
 */
void ContinuousServo::PIDSequence(float targetAngle) {
    float absAngle = this->encoder->readAngle(); // Increment the encoder angle
    double relAngle = this->encoder->getRelAngle();

    // PID Feedback
    float angleError = targetAngle - relAngle;
    this->Input = angleError;
    this->pidController->Compute();
    this->drivePWM(this->Output/3); // Divide by 3 to limit to 5V

    // Log messages
    if (millis() - lastPrint > 50000) {
        Serial.print(relAngle, 2);
        Serial.println(" deg (relativeAngle)");
        Serial.print("Angle read: ");
        Serial.println(absAngle);
        Serial.print("Target angle: ");
        Serial.println(targetAngle);
        Serial.print("Angle error: ");
        Serial.println(angleError);
        Serial.print("PID Output: ");
        Serial.println(Output);
        Serial.println("-----");
        lastPrint = millis();
    }

}

void ContinuousServo::updateStability() {
    float currentRelAngle = this->encoder->getRelAngle();
    if (abs(currentRelAngle - this->targetAngle) < this->tolerance) {
        this->stableCounter = constrain(this->stableCounter+1, 0, this->stableThreshold);
    } else {
        this->stableCounter = 0;
    }
}

bool ContinuousServo::reachedTarget() {
    // Serial.print("Stable counter: ");
    // Serial.println(this->stableCounter);
    return this->stableCounter >= this->stableThreshold;
}

/**
 * Run this inside void loop to enable servo feedback
 */
void ContinuousServo::loop() {
    unsigned long currentTime = millis();
    if (currentTime - this->lastPIDTime > this->PIDSampleTime) {
        this->PIDSequence(this->targetAngle);
        this->lastPIDTime = currentTime;
        this->updateStability();
    }
}


void ContinuousServo::testSequence() {
    unsigned long startTime = millis();
    // this->moveTo(60);
    // while (millis() - startTime < 1000) {
    //     this->loop();
    //     if (this->reachedTarget()) {
    //         Serial.println("Reached Target 1!!");
    //     }
    // }

    // delay(2000);
    // this->moveTo(360);
    // startTime = millis();
    // while (millis() - startTime < 1000) {
    //     this->loop();
    //         if (this->reachedTarget()) {
    //         Serial.println("Reached Target 2!!");
    //     }
    // }
    // delay(2000);

    // this->moveBy(720);
    //     startTime = millis();

    // while (millis() - startTime < 3000) {
    //     this->loop();
    //         if (this->reachedTarget()) {
    //         Serial.println("Reached Target 3!!");
    //     }
    // }

    for(int i = 0; i < 5; i++) {
        this->moveBy(360);
        this->loop();
        while (!this->reachedTarget()) {
            this->loop();

        }
        Serial.print("Reached Target: ");
        Serial.println(i);
        Serial.println(this->encoder->getRelAngle());

        delay(100);
    }

    // this->moveBy(360);
    // startTime = millis();
    // while (millis() - startTime < 1000) {
    //     this->loop();
    // }
}

void ContinuousServo::homingSequence() {
    this->encoder->home();
    this->moveTo(0);
    // if (dir == true) {
    //     this->moveBy(10);
    // } else {
    //     this->moveBy(10);
    // }
}


// PID Tuning
// int newKp = map(analogRead(P_Pin), 0, BIT_12_LIMIT, 0, 1000);
// double newKd = ((double) map(analogRead(D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;
// Serial.print("Kp: ");
// Serial.println(newKp);
// Serial.print("Kd: ");
// Serial.println(newKd);
// myPID.SetTunings(newKp, 0, newKd);

// Pk =  map(analogRead(34), 0, BIT_12_LIMIT, 0, 1000);
// Serial.print("Kp: ");
// Serial.println(Pk);

// Dk =  map(analogRead(38), 0, BIT_12_LIMIT, 0, 1000)/30;
// Serial.print("Kd: ");
// Serial.println(Dk);