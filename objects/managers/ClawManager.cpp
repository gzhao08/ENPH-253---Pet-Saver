// #include "ClawManager.h"

// /**
//  * ClawManager object, consists of a claw base, vertical stage, arm, grabber and basket
//  * 0 is left, 1 is right
//  * 0 is low, 1 is high
//  */
// ClawManager::ClawManager(){}

// /**
//  * Sets up claw manager
//  * @param base ClawBase object related to claw
//  * @param vertical ClawVerticalStage object related to claw
//  * @param arm ClawArm object related to claw
//  * @param grab ClawGrabber object related to claw
//  * @param basket Basket object related to claw
//  */
// void ClawManager::begin(ClawBase* base, ClawVerticalStage* vertical, ClawArm* arm, ClawGrabber* grab) {
//     this->base = base;
//     this->vertical = vertical;
//     this->arm = arm;
//     this->grab = grab;
// }

// /**
//  * Sets all degrees of freedom to home position: 
//  * base position = 0 (forward)
//  * vertical stage above basket but below door (test mon) 
//  * arm fully retracted
//  * claw closed
//  */
// void ClawManager::reset() {
//     this->grab->setPosition(this->grab->CLOSE);
//     this->grabPos = this->grab->CLOSE;

//     this->vertical->setPosition(this->vertical->RESET_HEIGHT);
//     this->verticalPos = this->vertical->RESET_HEIGHT;

//     this->arm->setPosition(this->arm->MIN); 
//     this->armPos = this->arm->MIN;

//     this->base->setPosition(0);
//     this->basePos = 0;
// }

// /**
//  * Extend arm to sensing position (for sensing while driving)
//  * @param direction of pets -- 0 is left, 1 is right
//  * @param height of pets -- 0 is low pets, 1 is high pets
//  */
// void ClawManager::sense(bool height, bool direction) {
//     //reset height
//     this->vertical->setPosition(this->vertical->RESET_HEIGHT);
//     this->verticalPos = this->vertical->RESET_HEIGHT;

//     //rotate, change height, extend
//     if (direction) {
//         this->base->setPosition(90);
//         this->basePos = 90;
//     }
//     else {
//         this->base->setPosition(-90);
//         this->basePos = -90;
//     }

//     if (height) {
//         this->vertical->setPosition(this->vertical->HIGH_SENSE);
//         this->verticalPos = this->vertical->HIGH_SENSE;
//     }
//     else {
//         this->vertical->setPosition(this->vertical->LOW_SENSE);
//         this->verticalPos = this->vertical->LOW_SENSE;
//     }

//     this->arm->setPosition(this->arm->MAX);
//     this->armPos = this->arm->MAX;
// }

// /**
//  * open claw, lower arm to pet height and close claw
//  * @param height 0 for low pets, 1 for high pets
//  */
// void ClawManager::grab(bool height) {
//     this->grab->setPosition(this->grab->OPEN);
//     this->grabPos = this->grab->OPEN;

//     if (height) {
//         this->vertical->setPosition(this->vertical->HIGH_PET);
//         this->verticalPos = this->vertical->HIGH_PET;
//     }
//     else {
//         this->vertical->setPosition(this->vertical->LOW_PET);
//         this->verticalPos = this->vertical->LOW_PET;
//     }    

//     this->grab->setPosition(this->grab->PET_CLOSE);
//     this->grabPos = this->grab->PET_CLOSE;
// }

// /**
//  * Deposit a pet in the basket (retract, vertical up, rotate to angleL, open, close)
//  * @param direction 0 is left, 1 is right
//  */
// void ClawManager::deposit(bool direction) {
//     this->arm->setPosition(this->arm->MIN);
//     this->armPos = this->arm->MIN; 
//     this->vertical->setPosition(this->vertical->RESET_HEIGHT);
//     this->verticalPos = this->vertical->RESET_HEIGHT;

//     if (direction) {
//         this->base->setPosition(this->LEFT_ANGLE);
//         this->basePos = this->LEFT_ANGLE;
//     }
//     else {
//         this->base->setPosition(this->RIGHT_ANGLE);    
//         this->basePos = this->RIGHT_ANGLE;
//     }

//     this->grab->setPosition(this->grab->OPEN);
//     this->grabPos = this->grab->OPEN;
//     this->grab->setPosition(this->grab->CLOSE);
//     this->grabPos = this->grab->CLOSE;
// }

// /**
//  * pick up pet from basket
//  * @param direction 0 is left, 1 is right
//  */
// void ClawManager::retrieve(bool direction) {
// //vertical to reset, retract, rotate to basket side
//     this->vertical->setPosition(this->vertical->RESET_HEIGHT);
//     this->verticalPos = this->vertical->RESET_HEIGHT; 

//     this->arm->setPosition(this->arm->MIN);
//     this->armPos = this->arm->MIN; 

//     if (direction) {
//         this->base->setPosition(this->RIGHT_ANGLE);
//         this->basePos = this->RIGHT_ANGLE;
//     }
//     else {
//         this->base->setPosition(this->LEFT_ANGLE);
//         this->basePos = this->LEFT_ANGLE;
//     }

// //open, lower, close
//     this->grab->setPosition(this->grab->OPEN);
//     this->grabPos = this->grab->OPEN; 

//     this->vertical->setPosition(this->vertical->MIN_HEIGHT);
//     this->verticalPos = this->vertical->MIN_HEIGHT; 

//     this->grab->setPosition(this->grab->PET_CLOSE);
//     this->grabPos = this->grab->PET_CLOSE;
// }

// /**
//  * Assumes window is perpendicular to claw (retract, rotate -90 if pos is 1 and 90 if pos is 0, extend fully, open, close, retract fully, rotate 0)
//  * @param pos is 0 if robot is right of window and 1 if robot is left of window
//  */
// void ClawManager::windowDrop(bool direction) {
// //retract and lift for rotation
// this->arm->setPosition(0);
// this->armPos = 0;
// this->vertical->setPosition(this->vertical->RESET_HEIGHT);
// this->verticalPos = this->vertical->RESET_HEIGHT;

// //rotate, lower, extend and drop
// if (direction) {
//     this->base->setPosition(-90);
//     this->basePos = -90;
// }
// else {
//     this->base->setPosition(90);
//     this->basePos = 90; 
// }
// this->vertical->setPosition(this->vertical->MIN_HEIGHT);
// this->verticalPos = this->vertical->MIN_HEIGHT;
// this->arm->setPosition(this->arm->MAX);
// this->armPos = this->arm->MAX;
// this->grab->setPosition(this->grap->OPEN);
// this->grabPos = this->grab->OPEN;

// //close and retract
// this->grab->setPosition(this->grab->PET_CLOSE);
// this->grabPos = this->grab->PET_CLOSE;
// this->arm->setPosition(this->arm->MIN);
// this->armPos = this->arm->MIN; 
// }
