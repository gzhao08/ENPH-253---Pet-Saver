#include "ClawManager.h"

/**
 * ClawManager object, consists of a claw base, vertical stage, arm, grabber and basket
 * 0 is left, 1 is right
 * 0 is low, 1 is high
 */

// int armMotorPin1, int armMotorPin2, int armPwmChannel1, int armPwmChannel2, int armMuxLine, bool armEncoderOnTerminalSide,
// int armSwitchPin, bool armNormallyOpen, int verticalStageMotorPin1, int verticalStageMotorPin2, int verticalStagePwmChannel1, int verticalStagePwmChannel2, int verticalStageMuxLine, bool verticalStageEncoderOnTerminalSide,
// int verticalStageSwitchPin, bool verticalStageNormallyOpen, int baseMotorPin1, int baseMotorPin2, int basePwmChannel1, int basePwmChannel2, int baseMuxLine, bool baseEncoderOnTerminalSide, 
// int baseSwitchPin, bool baseNormallyOpen, int grabberMotorPin, int grabberPwmChannel, int manager1, int manager2

// this->armMotorPin1, this->armMotorPin2, this->armPwmChannel1, this->armPwmChannel2, this->armMuxLine, this->armEncoderOnTerminalSide, 
// this->armSwitchPin, this->armNormallyOpen, this->verticalStageMotorPin1, this->verticalStageMotorPin2, this->verticalStagePwmChannel1, this->verticalStagePwmChannel2, this->verticalStageMuxLine, this->verticalStageEncoderOnTerminalSide,
// this->verticalStageSwitchPin, this->verticalStageNormallyOpen, this->baseMotorPin1, this->baseMotorPin2, this->basePwmChannel1, this->basePwmChannel2, this->baseMuxLine, this->baseEncoderOnTerminalSide, 
// this->baseSwitchPin, this->baseNormallyOpen, this->grabberMotorPin, this->grabberPwmChannel, 8, -1

//   arm(this->armMotorPin1, this->armMotorPin2, this->armPwmChannel1, this->armPwmChannel2, this->armMuxLine, this->armEncoderOnTerminalSide, 
//         this->armSwitchPin, this->armNormallyOpen),
//     vertical(this->verticalStageMotorPin1, this->verticalStageMotorPin2, this->verticalStagePwmChannel1, this->verticalStagePwmChannel2, this->verticalStageMuxLine, this->verticalStageEncoderOnTerminalSide,
//          this->verticalStageSwitchPin, this->verticalStageNormallyOpen),
//     base(this->baseMotorPin1, this->baseMotorPin2, this->basePwmChannel1, this->basePwmChannel2, this->baseMuxLine, this->baseEncoderOnTerminalSide, 
//          this->baseSwitchPin, this->baseNormallyOpen),
//     grabber(this->grabberMotorPin, this->grabberPwmChannel),
//     wireManager1(8),
//     wireManager2(-1)

// int armMotorPin1 = 0, int armMotorPin2 = 0, int armPwmChannel1 = 0, int armPwmChannel2 = 0, int armMuxLine = 0, bool armEncoderOnTerminalSide = 0,
// int armSwitchPin = 0, bool armNormallyOpen = 0, int verticalStageMotorPin1 = 0, int verticalStageMotorPin2 = 0, int verticalStagePwmChannel1 = 0, int verticalStagePwmChannel2 = 0, int verticalStageMuxLine = 0, bool verticalStageEncoderOnTerminalSide = 0,
// int verticalStageSwitchPin = 0, bool verticalStageNormallyOpen = 0, int baseMotorPin1 = 0, int baseMotorPin2 = 0, int basePwmChannel1 = 0, int basePwmChannel2 = 0, int baseMuxLine = 0, bool baseEncoderOnTerminalSide = 0, 
// int baseSwitchPin = 0, bool baseNormallyOpen = 0, int grabberMotorPin = 0, int grabberPwmChannel = 0, int manager1 = 0, int manager2 = 0

ClawManager::ClawManager(int armMotorPin1, int armMotorPin2, int armPwmChannel1, int armPwmChannel2, int armMuxLine, bool armEncoderOnTerminalSide,
int armSwitchPin, bool armNormallyOpen, int verticalStageMotorPin1, int verticalStageMotorPin2, int verticalStagePwmChannel1, int verticalStagePwmChannel2, int verticalStageMuxLine, bool verticalStageEncoderOnTerminalSide,
int verticalStageSwitchPin, bool verticalStageNormallyOpen, int baseMotorPin1, int baseMotorPin2, int basePwmChannel1, int basePwmChannel2, int baseMuxLine, bool baseEncoderOnTerminalSide, 
int baseSwitchPin, bool baseNormallyOpen, int grabberMotorPin, int grabberPwmChannel, int manager1, int manager2) :
    arm(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
        armSwitchPin, armNormallyOpen), 
    vertical(verticalStageMotorPin1, verticalStageMotorPin2, verticalStagePwmChannel1, verticalStagePwmChannel2, verticalStageMuxLine, verticalStageEncoderOnTerminalSide,
         verticalStageSwitchPin, verticalStageNormallyOpen),
    base(baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, 
          baseSwitchPin, baseNormallyOpen),
    grabber(grabberMotorPin, grabberPwmChannel),
    wireManager1(8),
    wireManager2(-1)
    {

}

/**
 * Sets up claw manager
 */
void ClawManager::begin() {
    this->wireManager1.begin(&Wire);
    this->wireManager2.begin(&Wire1);
    this->arm.begin(&wireManager1);
    this->vertical.begin(&wireManager1);
    this->base.begin(&wireManager2);
    this->grabber.begin();
}

void ClawManager::loop() {
    this->arm.loop();
    this->vertical.loop();
    this->base.loop();
}

void ClawManager::homingSequence() {
    this->arm.homingSequence();
    this->vertical.homingSequence();
    this->vertical.setPosition(50);
    while (!this->vertical.reachedTarget()) {
        this->vertical.loop();
    }
    this->base.homingSequence();
}

void ClawManager::setPositionArm(int pos) {
    this->arm.setPosition(pos);
}

void ClawManager::setPositionVertical(int pos) {
    this->vertical.setPosition(pos);
}

void ClawManager::setPositionBase(int pos) {
    this->base.setPosition(pos);
}

void ClawManager::setPositionGrabber(int pos) {
    this->grabber.setPositionDegrees(pos);
}

// ROUTINES //
/** 
 * 1. Drive position V -- input height
 * 2. Pick up low pet V
 *  a. coarse scan
 *  b. fine scan
 * 3. Retract pet NV
 * 4. Ramp Drop NV
 * 5. Deposit V
 * 6. Retrieve V
 * 7. Pick up high pet V
 * 7. Window Drop V
 */

/**
 * Set drive position
 * Sequence: Base forward, arm retracted, vertical stage at specified height, claw closed 
 * @param height position of vertical stage, set to VERTICAL_MIN for doorway
 */
void ClawManager::seqDrivePosition(float height) {
    this->arm.setPosition(this->ARM_MIN);
    this->base.setPosition(this->BASE_FORWARD);
    this->vertical.setPosition(height);
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE);
}
/*
 * Picks up pet assuming pet is directly below magnetometer 
 * Sequence: arm retracts, lower by VERTICAL_PICKUP, arm extends, grabber closes, raise to debris height
 */
void ClawManager::seqPickUpLow() {
    this->arm.moveBy(this->ARM_PICKUP_LOW); 
    this->vertical.moveBy(this->VERTICAL_PICKUP_LOW);
    this->arm.moveBy(-1 * this->ARM_PICKUP_LOW); 
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE);
    this->vertical.moveBy(this->VERTICAL_PET_ABOVE_CHASSIS); 
}

/*
 * Retracts pet to chassis once it has been picked up, does not deposit anywhere 
 * Sequence: retract arm, rotate base
 */
void ClawManager::seqRetract() { 
    this->arm.setPosition(this->ARM_MIN); 
    this->base.setPosition(this->BASE_FORWARD);
}

/*
 * Drops pet 1 off of ramp, assuming arm is closed around pet and retracted facing forward
 * Sequence: rotate base left, extend arm, open grabber, close grabber, retract arm, rotate base forward 
 */
void ClawManager::seqRampDrop() {
    this->base.setPosition(this->BASE_LEFT);
    this->arm.setPosition(this->ARM_MAX); 
    this->grabber.setPositionDegrees(this->GRABBER_OPEN);
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE);
    this->arm.setPosition(this->ARM_MIN); 
    this->base.setPosition(this->BASE_FORWARD);
}    

/**
 * deposits pet in basket assuming it won't hit another pet on the way, and has not been retracted  
 * Sequence: retract arm, rotate to basket position, lower to min, open, raise to above pet head height, 
 * @param direction left or right of basket; 0 is left, 1 is right 
 */    
void ClawManager::seqDeposit(bool direction) {
    this->arm.setPosition(this->ARM_MIN);

    if (direction) {
        this->base.setPosition(this->BASE_LEFT_BASKET);
    }
    else {
        this->base.setPosition(this->BASE_RIGHT_BASKET);
    }

    this->vertical.setPosition(this->VERTICAL_MIN); 
    this->grabber.setPositionDegrees(this->GRABBER_BASKET_OPEN); 
    this->vertical.setPosition(this->VERTICAL_PET_ABOVE_CHASSIS);
}

/**
 * retrieves pet from basket and lifts above chassis
 * sequence: raise arm, rotate, open, lower to min, close, raise  
 * @param direction left or right of basket; 0 is left, 1 is right
 */
void ClawManager::seqRetrieve(bool direction) {
    this->vertical.setPosition(this->VERTICAL_PET_ABOVE_CHASSIS);
    
    if (direction) {
        this->base.setPosition(this->BASE_LEFT_BASKET);
    }
    else {
        this->base.setPosition(this->BASE_RIGHT_BASKET);
    }

    this->grabber.setPositionDegrees(this->GRABBER_BASKET_OPEN);
    this->vertical.setPosition(this->VERTICAL_MIN);
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE);
    this->vertical.setPosition(this->VERTICAL_PET_ABOVE_CHASSIS);
}

/**
 * Sequence: open to pos, raise, extend into pillar, close, retract from pillar
 * @param chute false for pet three, true for pet four
 */
void ClawManager::seqPickUpHigh(bool chute) {
    this->base.setPosition(this->BASE_LEFT);
    if (chute) {
        this->grabber.setPositionDegrees(this->GRABBER_CHUTE_OPEN);
    }
    else {
        this->grabber.setPositionDegrees(this->GRABBER_OPEN);        
    }
    this->vertical.setPosition(this->VERTICAL_MAX);
    this->arm.moveBy(this->ARM_HIGH_GRAB);
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE); 
    this->arm.setPosition(this->ARM_MIN);
    this->base.setPosition(this->BASE_FORWARD);
    this->vertical.setPosition(this->VERTICAL_PET_ABOVE_CHASSIS);
}

/**
 * Sequence: retract, rotate, extend, open, close, retract, rotate
 * @param direction 0 is left, 1 is right 
 */
void ClawManager::seqWindowDrop(bool direction) {
    this->arm.setPosition(this->ARM_OUTSIDE_CHASSIS);
    this->vertical.setPosition(this->VERTICAL_DEBRIS_HEIGHT);

    if (direction) {
        this->base.setPosition(this->BASE_LEFT);
    }
    else {
        this->base.setPosition(this->BASE_RIGHT);
    }
    
    this->arm.setPosition(this->ARM_MAX);
    this->grabber.setPositionDegrees(this->GRABBER_OPEN);
    delay(1000);
    this->grabber.setPositionDegrees(this->GRABBER_CLOSE);
    this->arm.setPosition(this->ARM_MIN);
    this->base.setPosition(this->BASE_FORWARD);
}

