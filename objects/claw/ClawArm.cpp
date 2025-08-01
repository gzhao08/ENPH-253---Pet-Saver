//151mm (~6") per rotation of horizontal gear (d = 48mm)

#include "ClawArm.h"

/**
 * ClawArm object, consists of a continuous servo and a microswitch
 */
ClawArm::ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
    int switchPin, bool normallyOpen) : 
    ClawPart(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide, 
        switchPin, normallyOpen) {
        
    this->partName = this->_partName; // For debugging purposes

    // Conversion and positions
    this->ENCODER_TO_POS_CONVERSION = this->_ENCODER_TO_POS_CONVERSION;
    this->POS_TO_ENCODER_CONVERSION = this->_POS_TO_ENCODER_CONVERSION;
    this->ABS_POS_LIMIT = this->_ABS_POS_LIMIT; // Position limit (for homing purposes)
    this->MIN_POSITION = this->_MIN_POSITION;
    this->MAX_POSITION = this->_MAX_POSITION;

    // PID Parameters
    this->Pk = this->_Pk;
    this->Dk = this->_Dk;

    // Servo Parameters
    this->servoMaxVoltage = this->_servoMaxVoltage;
    this->servoTolerance = this->_servoTolerance; // How much can angle deviate from target
}

/**
 * Test sequence to see if ClawArm is working
 */
void ClawArm::testSequence() {
    int actionDelay = 500;
    DelayManager timeoutTimer(2000);

    Serial.println("Claw Arm test sequence started");
    this->setPosition(0);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 0 timed out");
            break;
        }
    }
    Serial.println("Reached target position 0");
    delay(actionDelay);

    this->setPosition(50);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 50 timed out");
            break;
        }
    }
    Serial.println("Reached target position 50");
    delay(actionDelay);

    this->setPosition(100);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 100 timed out");
            break;
        }
    }
    Serial.println("Reached target position 100");
    delay(actionDelay);

    this->setPosition(150);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 150 timed out");
            break;
        }
    }
    Serial.println("Reached target position 150");
    delay(actionDelay);

    this->setPosition(200);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 200 timed out");
            break;
        }
    }
    Serial.println("Reached target position 200");
    delay(actionDelay);

    this->setPosition(10);
    timeoutTimer.reset();
    while (!this->reachedTarget()) {
        this->loop();
        if (timeoutTimer.checkAndReset()) {
            Serial.println("Position 10 timed out");
            break;
        }
    }
    Serial.println("Reached target position 10");
    Serial.println("Claw Arm test sequence done");
}
