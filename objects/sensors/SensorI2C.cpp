#include "SensorI2C.h"

SensorI2C::SensorI2C(int address, int muxLine) {
    this->address = address;
    this->muxLine = muxLine;
}

/**
 * Assign the WireManager instance to the sensor.
 * @param wireManager WireManager instance
 */
void SensorI2C::assignWireManager(WireManager* wireManager) {
    this->wireManager = wireManager;
}

/**
 * Begin tranmission to the I2C device at the sensor address.
 * @return status code of the transmission
 */
void SensorI2C::beginTransmission() {
    this->wireManager->beginTransmission(this->muxLine, this->address);
}

/**
 * End transmission to the I2C device at the sensor address.
 * @param stop Whether to send a stop message (release the bus)
 * @return status code of the transmission
 */
int SensorI2C::endTransmission(bool stop) {
    return this->wireManager->endTransmission(this->muxLine, stop);
    // Implementation for ending transmission
    // This would typically involve releasing the I2C bus
}

/**
 * Request data from the I2C device at the specified register address.
 * @param quantity The number of bytes to read
 * @param stop Whether to send a stop message (release the bus)
 * @return status code of the transmission
 */
int SensorI2C::requestFrom(int quantity, int stop) {
    return this->wireManager->requestFrom(this->muxLine, this->address, quantity, stop);
}

/**
 * Check how many bytes of data are available to read from the I2C device.
 * @return number of bytes available
 */
int SensorI2C::available() {
    return this->wireManager->available(this->muxLine);
}

/**
 * Write data to the I2C device.
 * @return status code of the write operation
 */
int SensorI2C::write(int value) {
    return this->wireManager->write(this->muxLine, value);
}

/**
 * Read a byte from the I2C device.
 * @return the byte read from the device
 */
uint8_t SensorI2C::read() {
    return this->wireManager->read(this->muxLine);
}

