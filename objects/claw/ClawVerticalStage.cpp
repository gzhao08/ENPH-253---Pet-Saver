#include "ClawVerticalStage.h"

/**
 * ClawVerticalStage object, consists of a continuous servo and a microswitch
 */
ClawVerticalStage::ClawVerticalStage(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
motorVertical(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
mswitchVertical(switchPin, normallyOpen) {}

/**
 * Sets up magnetic encoder
 * @param motorVertical Continuous servo object related to vertical stage
 * @param mswitchVertical microswitch object related to vertical stage
 */
void ClawVerticalStage::begin(WireManager* wireManager) {
    motorVertical.begin(wireManager);
    mswitchVertical.begin();
    motorVertical.setPDTuning(this->PID_P, this->PID_D);
    motorVertical.setMaxVoltage(8); // My max duty cycle is 0.8 * duty cycle mapping
    // Serial.println("motorVertical max duty cycle: " + String(motorVertical.getMaxDutyCycle()));
}

void ClawVerticalStage::setAsHome() {
    this->motorVertical.setAsHome();
}

/**
 * Home the claw vertical stage by moving it until the microswitch is pressed
 * This will set the home position of the claw vertical stage
 */
void ClawVerticalStage::homingSequence() {
    this->motorVertical.moveBy(18000); // Move down to ensure we are not at the top
    while (!this->mswitchVertical.isPressed()) {
        this->loop();
    }  
    this->setAsHome();
    Serial.println("Homing sequence done");
}

float ClawVerticalStage::getPosition() {
    float angle = this->motorVertical.getAngle(); 
    return angle * this->ANGLE_TO_MM_CONVERSION; // convert to mm

}

/**
 * raises claw to desired height
 * @param height above minimum height in mm
 */
void ClawVerticalStage::setPosition(float position) {
    //CW moves mechanism up
    float movePosition = constrain(position, this->MIN_HEIGHT, this->MAX_HEIGHT);
    float relAngle = movePosition * this->MM_TO_ANGLE_CONVERSION; // convert to angle in degrees
    this->motorVertical.moveTo(relAngle); //negative is CW, CW is up
}

void ClawVerticalStage::loop() {
    this->motorVertical.loop();
}

/**
 * @return true if the claw vertical stage has reached the target position, false otherwise
 */
bool ClawVerticalStage::reachedTarget() {
    return this->motorVertical.reachedTarget();
}

void ClawVerticalStage::setPIDTuningMode(bool mode) {
    this->motorVertical.setPIDTuningMode(mode);
}

void ClawVerticalStage::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->motorVertical.setPIDTuningPins(P_Pin, D_Pin);
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

