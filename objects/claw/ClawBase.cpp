#include "ClawBase.h"

/**
 * ClawBase object, consists of a continuous servo and a microswitch
 */
ClawBase::ClawBase(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
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
void ClawBase::testSequence() {

}

