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
    this->motorArm.setPDTuning(this->Pk, this->Dk);
}

/**
 * Set current position as home
 */
void ClawArm::setAsHome() {
    this->motorArm.setAsHome();
}

/**
 * Sequence to home ClawArm.
 * Make sure that MM_TO_ANGLE_CONVERSION is correct! Especially the signs
 */
void ClawArm::homingSequence() {
    int overRetractionAngle = -this->ABS_POS_LIMIT * this->MM_TO_ANGLE_CONVERSION * 1.2;
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

/**
 * Get current position of the claw
 */
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

/**
 * Call in void loop to actuate Claw
 */
void ClawArm::loop() {
    this->motorArm.loop();
}


void ClawArm::setPIDTuningMode(bool mode) {
    this->motorArm.setPIDTuningMode(mode);
}

void ClawArm::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->motorArm.setPIDTuningPins(P_Pin, D_Pin);
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
