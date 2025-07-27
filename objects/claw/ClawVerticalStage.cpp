#include "ClawVerticalStage.h"

/**
 * ClawVerticalStage object, consists of a continuous servo and a microswitch
 */
ClawVerticalStage::ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
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

void ClawVerticalStage::testSequence() {

    Serial.println("Claw Vertical Stage test sequence started");
    this->setPosition(0);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 0");
    delay(1000);
    this->setPosition(50);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 50");
    delay(1000);
    this->setPosition(100);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 100");
    delay(1000);
    this->setPosition(150);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 150");
    delay(1000);
    this->setPosition(200);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 200");
    delay(1000);
    this->setPosition(50);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 50 again");
    delay(1000);
    this->setPosition(0);
    while (!this->reachedTarget()) {
        this->loop();
    }
    Serial.println("Reached target position 0 again");
    Serial.println("Claw Arm test sequence done");
}

