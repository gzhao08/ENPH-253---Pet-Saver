#include "MagneticEncoder.h"

/**
 * Object representing a magnetic encoder
 */
MagneticEncoder::MagneticEncoder(){}

/**
 * @param wireObj   Wire object related to I2C line being used
 */
void MagneticEncoder::begin(TwoWire* wireObj) {
    this->wire = wireObj;
}

/**
 * Read raw angle from magnetic encoder
 * @return raw angle value from 0 to 4096 IF success
 * @return 0xFFFF IF fail
 */
uint16_t MagneticEncoder::readRawAngle() {
    this->wire->beginTransmission(this->AS5600_ADDR);
    this->wire->write(this->AS5600_MSG_REG);

    if (this->wire->endTransmission(false) != 0) { 
        return 0xFFFF; // Transmission failed
    }

    this->wire->requestFrom(AS5600_ADDR, 2); // Request MSB and LSB

    if (this->wire->available() == 2) {
        uint8_t msb = this->wire->read();
        uint8_t lsb = this->wire->read();
        return ((uint16_t) msb << 8) | lsb;
    }

    return 0xFFFF;
}

/**
 * Calculate the angle based on magnetic encoder output
 * @return angle value from 0 to 360 IF success
 * @return -1 IF something went wrong
 */
double MagneticEncoder::readAngle() {
  uint16_t angle = readRawAngle();
  if (angle != 0xFFFF) {
    return (angle * 360.0) / 4096.0;
  } else {
    return -1;
  }
}

/**
 * Calculates the difference of toAngle and fromAngle
 * Imagine them being vectors and dif = toAngle - fromAngle
 * @param toAngle 
 * @param fromAngle
 * @return a value between -180 and 180. 
 * Positive if increasing fromAngle to toAngle causes a smaller rotation than decreasing fromAngle
 */
double MagneticEncoder::angleDifference(double toAngle, double fromAngle) {
  float diff = toAngle - fromAngle;
  // Make sure it's between -360 to 360
  diff = fmod(diff, 360);

  if (diff > 180) {
      diff -= 360;
  } else if (diff < -180) {
      diff += 360;
  }

  return diff;
}



