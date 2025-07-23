#include "ContinuousServo.h"

unsigned long lastPrint = 0;
/**
 * ContinuousServo object, consists of a motor and a magnetic encoder
 * This is a RELATIVE ANGLE based application (not the absolute reading of the encoder)
 * Initializes encoder, motor, and PID controller
 */
ContinuousServo::ContinuousServo(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide) 
: encoder(muxLine), motor(motorPin1, motorPin2, pwmChannel1, pwmChannel2, 5), pidTuningDelayManager(1000)
 {
    /**
     * Working:
     * - Encoder increases when magnet is rotated counter-clockwise
     * - When looking at the side of the motor with the terminals, driving IN1 HIGH will rotate the motor clockwise.
     * - Motor driven with PID output. PID output is + when want to increase the encoder output
     * - When encoder is on the terminal side, IN1 high will decrease the encoder output
     * - Therefore we need to multiply the PID output by -1 when the encoder is on the terminal side
     */
    if (encoderOnTerminalSide) {
        this->DIRECTION_MULTIPLIER = -1;
    } else {
        this->DIRECTION_MULTIPLIER = 1;
    }
}

/**
 * Sets up the ContinuousServo
 */
void ContinuousServo::begin(WireManager* wireManager) {
    // Motor setup
    motor.begin();

    // Encoder setup
    this->encoder.begin(wireManager);
    this->targetAngle = this->encoder.getRelAngle();


    // PID controller setup
    this->pidController = new PID(&this->Input, &this->Output, &this->Setpoint, this->Pk, this->Ik, this->Dk, DIRECT);
    this->pidController->SetMode(AUTOMATIC);
    this->pidController->SetOutputLimits(-this->motor.getMaxDutyCycle(), this->motor.getMaxDutyCycle()); // Set output limits to motor max duty cycle
    this->pidController->SetSampleTime(this->PIDSampleTime);
}

/**
 * Move the servo by a certain number of degrees
 * This is a relative movement, so it will add the degrees to the current relative angle
 * @param degrees can be positive or negative
 */
void ContinuousServo::moveBy(float degrees) {
    this->stableCounter = 0;
    this->targetAngle = this->encoder.getRelAngle() + degrees;
}

/**
 * Move the servo to a certain absolute angle
 * @param degrees can be positive or negative
 */
void ContinuousServo::moveTo(float degrees) {
    this->stableCounter = 0;
    this->targetAngle = degrees;
}

/**
 * Return the current angle of the servo
 * This is the relative angle of the servo in respect to the home position
 * @return relative angle in degrees
 */
float ContinuousServo::getAngle() {
    return this->encoder.getRelAngle();
}

/**
 * PID Sequence uses relative encoder values
 */
void ContinuousServo::PIDSequence(float targetAngle) {
    float absAngle = this->encoder.readAngle();
    float relAngle = this->encoder.getRelAngle();

    // PID Feedback
    float angleError = targetAngle - relAngle;
    this->Input = angleError;
    this->pidController->Compute();
    this->motor.drivePWM(this->Output * this->DIRECTION_MULTIPLIER); // Divide by 3 to limit to 5V

    // Log messages
    if (millis() - lastPrint > 1000) {
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

/**
 * Update the stability counter based on the current relative angle
 */
void ContinuousServo::updateStability() {
    float currentRelAngle = this->encoder.getRelAngle();
    if (abs(currentRelAngle - this->targetAngle) < this->tolerance) {
        this->stableCounter = constrain(this->stableCounter + 1, 0, this->stableThreshold);
    } else {
        this->stableCounter = 0;
    }
}

/**
 * Check if the servo has reached its target position
 */
bool ContinuousServo::reachedTarget() {
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

        if (this->PIDTuningMode) {
            this->tunePID();
        }
    }
}

/**
 * Sequence to test if the servo is working or not
 */
void ContinuousServo::testSequence() {
    DelayManager sequenceDelay(3000);
    sequenceDelay.reset();
    
    this->moveTo(60);
    while (true) {
        this->loop();

        if (this->reachedTarget()) {
            Serial.println("Reached Target 1!!");
        }

        if (sequenceDelay.checkAndReset()) {
            Serial.println("Target 1 timeout, moving to next target");
            break;
        }
    }

    delay(500);

    this->moveTo(360);
    while (true) {
        this->loop();
        if (this->reachedTarget()) {
            Serial.println("Reached Target 2!!");
            break;
        }

        if (sequenceDelay.checkAndReset()) {
            Serial.println("Target 2 timeout, moving to next target");
            break;
        }
    }
    delay(500);

    this->moveBy(540);
    while (true) {
        this->loop();
        if (this->reachedTarget()) {
            Serial.println("Reached Target 3!!");
            break;
        }

        if (sequenceDelay.checkAndReset()) {
            Serial.println("Target 3 timeout, moving to next target");
            break;
        }
    }

    Serial.println("Multiple rotation sequence: ");
    for (int i = 0; i < 3; i++) {
        this->moveBy(360);
        this->loop();
        while (!this->reachedTarget()) {
            this->loop();

        }
        Serial.print("Reached Target: ");
        Serial.println(i);
        Serial.println(this->encoder.getRelAngle());

        delay(100);
    }
}

/**
 * Set the current angle as home
 * This also makes the servo want to stay at the current angle
 * This is useful for homing the servo to a known position
 */
void ContinuousServo::home() {
    this->encoder.home();
    this->moveTo(0);
}

/**
 * Set the PID tuning mode
 */
void ContinuousServo::setPIDTuningMode(bool mode) {
    this->PIDTuningMode = mode;
}

/**
 * Set the PD tuning pins
 */
void ContinuousServo::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->P_Pin = P_Pin;
    this->D_Pin = D_Pin;
}

/**
 * Tune the PID controller using the analog pins
 */
void ContinuousServo::tunePID() {
    // PID Tuning
    double newKp = map(analogRead(this->P_Pin), 0, BIT_12_LIMIT, 0, 1000);
    double newKd = ((double) map(analogRead(this->D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;

    if (pidTuningDelayManager.checkAndReset()) {
        Serial.print("Kp: ");
        Serial.println(newKp);
        Serial.print("Kd: ");
        Serial.println(newKd);
    }

    this->pidController->SetTunings(newKp, 0, newKd);
}