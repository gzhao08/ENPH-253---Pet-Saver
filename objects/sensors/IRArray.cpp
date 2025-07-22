// #include "IRArray.h"

// /**
//  * IRArray object, consists of 4 IR sensors
//  * <0 (negative) is left, >0 (positive) is right
//  * 0 is low, 1 is high
//  */

// IRArray() {
//     error = 0;
//     lastError = 0;
// }

// void IRArray::begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin) {
//     pin1 = outerLeftPin;
//     pin2 = innerLeftPin;
//     pin3 = innerRightPin;
//     pin4 = outerRightPin;

//     pinMode(pin1, INPUT);
//     pinMode(pin2, INPUT);
//     pinMode(pin3, INPUT);
//     pinMode(pin4, INPUT);
// }

// void IRArray::takeReading(boolean show) {
//     r1 = !digitalRead(pin1);
//     r2 = !digitalRead(pin2);
//     r3 = !digitalRead(pin3);
//     r4 = !digitalRead(pin4);
//     if (show) {
//         showReading();
//     }
// }

// void IRArray::showReading() {
//     Serial.printf("1: %d, 2: %d, 3: %d, 4: %d\n", r1, r2, r3, r4);
// }

// void IRArray::showState() {
//     showReading();
//     Serial.printf("Error: %d, Last Error: %d\n", error, lastError);
// }

// int IRArray::getError() {

//     if (r2&&r3) {error = 0;// on the line
//     } else if (r1||r2) { // on left side
//         if (!r1 && r2) { // inner left
//             error = -ERR_1;            
//         }
//         else if (r1 && r2) { // mid left
//             error = -ERR_2;
//         }
//         else if (r1 && !r2) { // outer left
//             error = -ERR_3;
//         }
//     }

//     else if (r3||r4) { // on right side
//         if (!r4 && r3) { // inner right
//             error = ERR_1;            
//         }
//         else if (r4 && r3) { // mid right
//             error = ERR_2;
//         }
//         else if (r4 && !r3) { // outer right
//             error = ERR_3;
//         }
//     }

//     else if (!r1&&!r2&&!r3&&!r4) { // totally off line
//         if (lastError < 0) { // on the left
//             error = -ERR_4;
//         }

//         else {
//             error = ERR_4;
//         }   
//     }

//     return error;
// }

// void IRArray::update() {
//     lastError = error;
//     return;
// }