//151mm (~6") per rotation of horizontal gear (d = 48mm)

#include "ClawArm.h"

/**
 * ClawArm object, consists of a continuous servo and a microswitch
 */
ClawArm::ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, 
    int switchPin, bool normallyOpen) : 
motorArm(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
mswitchArm(switchPin, normallyOpen) {}

/**
 * Sets up magnetic encoder
 * @param motorArm Continuous servo object related to claw arm
 * @param mswitchArm microswitch object related to claw arm
 */
void ClawArm::begin(WireManager* wireManager) {
    this->motorArm.begin(wireManager);
    this->mswitchArm.begin();
    this->motorArm.setPDTuning(55, 0.77);
}

void ClawArm::setAsHome() {
    this->motorArm.setAsHome();
}

void ClawArm::homingSequence() {
    int overRetractionAngle = -227 / this->ANGLE_TO_MM_CONVERSION * 1.2; // 227mm is the distance from home to the end of the claw arm
    this->motorArm.moveBy(overRetractionAngle);

    while (!this->mswitchArm.isPressed()) {
        this->motorArm.loop();
    }
    this->setAsHome();
    Serial.println("Arm Homing sequence done");
}

/**
 * @return true if the claw arm has reached the target position, false otherwise
 */
float ClawArm::reachedTarget() {
    return this->motorArm.reachedTarget();
}

float ClawArm::getPosition() {
    float angle = this->motorArm.getAngle(); 
    return angle * this->ANGLE_TO_MM_CONVERSION;
}

/**
 * @param position final position of horizontal stage in mm from home position (0)
 */
void ClawArm::setPosition(float position) {
    // Limit the position
    float movePosition = constrain(position, this->MIN_POSITION, this->MAX_POSITION);
    float relAngle = movePosition * this->MM_TO_ANGLE_CONVERSION; // convert to angle in degrees
    this->motorArm.moveTo(relAngle);
}


void ClawArm::loop() {
    this->motorArm.loop();
}

void ClawArm::testSequence() {
    Serial.println("Claw Arm test sequence started");
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

void ClawArm::setPIDTuningMode(bool mode) {
    this->motorArm.setPIDTuningMode(mode);
}

void ClawArm::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->motorArm.setPIDTuningPins(P_Pin, D_Pin);
}

