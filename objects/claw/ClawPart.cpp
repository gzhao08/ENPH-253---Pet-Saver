#include "ClawPart.h"

/**
 * A claw part, consisting of a continuous servo and a microswitch
 */
ClawPart::ClawPart(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
continuousServo(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), 
microswitch(switchPin, normallyOpen),
reachTargetTimeout(7000) {}

/**
 * Sets up claw part
 */
void ClawPart::begin(WireManager* wireManager) {
    this->continuousServo.begin(wireManager);

    this->continuousServo.setPDTuning(this->Pk, this->Dk);
    this->continuousServo.setMaxVoltage(this->servoMaxVoltage); 
    this->continuousServo.tolerance = this->servoTolerance;

    this->microswitch.begin();

    this->speed = 0;
    this->lastPosition = this->getPosition();
    this->lastPositionTime = millis();
}

/**
 * Sets current position as home
 */

void ClawPart::setAsHome() {
    this->continuousServo.setAsHome();
    this->setPosition(this->MIN_POSITION);
}

/**
 * Home the claw by moving it until the microswitch is pressed
 * This will set the home position of the claw part
 */
void ClawPart::homingSequence() {
    this->continuousServo.setMaxVoltage(this->servoMaxVoltage/1.4);

    int overshootAngle = -(this->ABS_POS_LIMIT - this->MIN_POSITION) * POS_TO_ENCODER_CONVERSION * 1.2;
    this->continuousServo.moveBy(overshootAngle);
    
    DelayManager timeout(5000);
    timeout.reset();
    while (!this->microswitch.isPressed()) {
        this->loop();
        if (timeout.checkAndReset()) {
            Serial.println("Homing timed out (10 seconds) for " + this->partName);
            break;
        }
    }  

    this->setAsHome();

    this->continuousServo.setMaxVoltage(this->servoMaxVoltage);
    timeout.reset();
    this->setPosition(this->MIN_POSITION + (this->MAX_POSITION-this->MIN_POSITION) * 0.05);
    while (!this->reachedTarget()) {
        this->loop();
        if (timeout.checkAndReset()) {
            Serial.println("Adjust position after homing timed out (10 seconds) for " + this->partName);
            break;
        }
    }
    Serial.println("Position adjusted");

    this->continuousServo.setMaxVoltage(this->servoMaxVoltage/1.4);

    int rehomeAngle = -(this->ABS_POS_LIMIT - this->MIN_POSITION) * POS_TO_ENCODER_CONVERSION * 0.2;
    this->continuousServo.moveBy(rehomeAngle); // Gentle rehome
    
    timeout.reset();
    while (!this->microswitch.isPressed()) {
        this->loop();
        if (timeout.checkAndReset()) {
            Serial.println("Homing timed out (10 seconds) for " + this->partName);
            break;
        }
    }  

    this->setAsHome();

    this->continuousServo.setMaxVoltage(this->servoMaxVoltage);
    timeout.reset();
    this->setPosition(this->MIN_POSITION + (this->MAX_POSITION-this->MIN_POSITION) * 0.05);
    while (!this->reachedTarget()) {
        this->loop();
        if (timeout.checkAndReset()) {
            Serial.println("Adjust position after homing timed out (10 seconds) for " + this->partName);
            break;
        }
    }
    Serial.println("Position adjusted");

    Serial.println("Current position of " + this->partName + " is set as home");


    Serial.println("Homing sequence done");
    this->continuousServo.setMaxVoltage(this->servoMaxVoltage);

}

/**
 * @return true if the claw part has reached the target position, false otherwise
 */
bool ClawPart::reachedTarget() {
    return this->continuousServo.reachedTarget();
}

/**
 * A blocking function that waits for the claw part to reach its target
 * @param timeout in milliseconds
 */
void ClawPart::waitToReachTarget(int timeout) {
    this->reachTargetTimeout.setTimeInterval(timeout);
    this->reachTargetTimeout.reset();

    while (!this->reachedTarget() && !this->reachTargetTimeout.isElapsed()) {
        this->loop();
    }
}

/**
 * @return true if the claw part is within some distance from the target position
 */
bool ClawPart::almostReachedTarget(int distance) {
    return abs(this->getPosition() - this->targetPosition) <= distance;
}

/**
 * Get position of claw part
 */
 float ClawPart::getPosition() {
    float angle = this->continuousServo.getAngle();
    return (angle * this->ENCODER_TO_POS_CONVERSION) + this->MIN_POSITION;
 }

 /** 
  * Move the claw part to the desired position
 */
void ClawPart::setPosition(float position) {
    float constrainedPosition = constrain(position, this->MIN_POSITION, this->MAX_POSITION);
    float shiftedPosition = constrainedPosition - this->MIN_POSITION;
    float relAngle = shiftedPosition * this->POS_TO_ENCODER_CONVERSION;

    this->targetPosition = position;
    // Serial.println("Set position in " + this->partName + " called: " + String(constrainedPosition));
    // Serial.println(shiftedPosition);
    // Serial.println(relAngle);
    this->continuousServo.moveTo(relAngle);
}

void ClawPart::moveBy(float movement) {
    float currentPosition = this->getPosition();
    this->setPosition(currentPosition+movement);
}
/**
 * Updates the speed value of the claw part
 */
void ClawPart::updateSpeed() {
    unsigned long currentTime = millis();
    float currentPosition = this->getPosition();
    this->speed = (currentPosition - this->lastPosition) / (currentTime - this->lastPositionTime);
    this->lastPositionTime = currentTime;
    this->lastPosition = currentPosition;
}

/**
 * Get speed in mm/s
 */
float ClawPart::getSpeed() {
    return this->speed;
}

void ClawPart::stop() {
    this->continuousServo.stop();
}

/**
 * Set PID tuning mode to true or false
 */
void ClawPart::setPIDTuningMode(bool mode) {
    this->continuousServo.setPIDTuningMode(mode);
}

/**
 * Set P and D tuning pin
 */
void ClawPart::setPIDTuningPins(int P_Pin, int D_Pin) {
    this->continuousServo.setPIDTuningPins(P_Pin, D_Pin);
}

/**
 * Call this in void loop to enable claw actuation
 */
void ClawPart::loop() {
    this->continuousServo.loop();
    this->updateSpeed();
}
