#include "MagneticEncoder.h"

MagneticEncoder::MagneticEncoder(TwoWire& wireObj) : wire(wireObj) {}

/**
 * Read raw angle from sensor
 * WireObj depends on which I2C channel is used
 */
uint16_t MagneticEncoder::readRawAngle() {
    this->wire.beginTransmission(this->AS5600_ADDR);
    this->wire.write(this->AS5600_MSG_REG);

    if (this->wire.endTransmission(false) != 0) { 
        return 0xFFFF; // Transmission failed
    }

    this->wire.requestFrom(AS5600_ADDR, 2); // Request MSB and LSB

    if (this->wire.available() == 2) {
        uint8_t msb = this->wire.read();
        uint8_t lsb = this->wire.read();
        return ((uint16_t) msb << 8) | lsb;
    }

    return 0xFFFF;
}

/**
 * Reads angle 
 * WireObj depends on which I2C channel is used
 * Output range: 0 to 360
 * Something is wrong, Output = -1 
 */
double MagneticEncoder::readAngle() {
  uint16_t angle = readRawAngle();
  if (angle != 0xFFFF) {
    return (angle * 360.0) / 4096.0;
  } else {
    return -1;
  }
}





