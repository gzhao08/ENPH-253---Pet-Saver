//151mm (~6") per rotation of horizontal gear (d = 48mm)

#include "ClawArm.h"

/**
 * ClawArm object, consists of a continuous servo and a microswitch
 */
ClawArm::ClawArm(int motorPin1, int motorPin2, int pwmChannel1, int pwmChannel2, int muxLine, bool encoderOnTerminalSide, int switchPin, bool normallyOpen) : 
motorArm(motorPin1, motorPin2, pwmChannel1, pwmChannel2, muxLine, encoderOnTerminalSide), mswitchArm(switchPin, normallyOpen) {}

/**
 * Sets up magnetic encoder
 * @param motorArm Continuous servo object related to claw arm
 * @param mswitchArm microswitch object related to claw arm
 */
void ClawArm::begin(WireManager* wireManager) {
    this->motorArm.begin(wireManager);
    this->mswitchArm.begin();
}

void ClawArm::home() {
    bool home = false; 
    while (!home) {
        this->motorArm.moveBy(500);
        //if(this->motorArm.reachedTarget()) {
            //this->motorArm.moveBy(10);
        //}
        if (this->mswitchArm.isPressed()) {
            this->motorArm.home();
            home = true;
        }
        
    }
}

float ClawArm::getPosition() {
    float angle = this->motorArm.getAngle(); 
    return angle * 151 / 360;
}

/**
 * @param position final position of horizontal stage in mm from home position (0)
 */
void ClawArm::setPosition(float position) {
    if (this->MIN <= position && this->MAX >= position) {
        float angle = position * 360 / 151;
        this->motorArm.moveTo(angle); 
    }
}


void ClawArm::loop() {
    this->motorArm.loop();
}

void ClawArm::testSequence() {

}

