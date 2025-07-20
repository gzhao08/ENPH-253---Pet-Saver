#include "Basket.h"

/**
 * Basket object, consists of a servo
 */
Basket::Basket(){}

/**
 * Sets up magnetic encoder
 * @param motor servo object related to basket
 */
void Basket::begin(Servo* motor) {
    this->motor = motor;
}

/**
 * resets basket mechanism
 */
void Basket::reset() {
    this->motor->writePosition(0);
}

/**
 * releases basket mechanism
 */ 
void Basket::release() {
    this->motor->writePosition(90); 
}
