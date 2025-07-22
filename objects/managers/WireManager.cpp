#include "WireManager.h"

/**
 * WireManager constructor.
 * @param muxPin The pin used to control the multiplexer. If not muxing, set to -1.
 */
WireManager::WireManager(int muxPin) {
    this->muxPin = muxPin;
}

/**
 * Initialize the WireManager with a TwoWire object.
 * @param wireObj The TwoWire object to use for I2C communication.
 * You should call Wire.begin() too.
 */
void WireManager::begin(TwoWire* wireObj) {
    if (this->muxPin != -1) {
        pinMode(this->muxPin, OUTPUT);
        digitalWrite(this->muxPin, HIGH); // Set the mux to default line (1)
    }
    this->wire = wireObj;
}

/**
 * Switch the multiplexer line to the specified muxLine.
 * @param muxLine The line to switch to (0 or 1). If -1 is passed, it does nothing (no muxing).
 */
// MuxLine 1 is JM1
// MuxLine 0 is JM2
void WireManager::switchMuxLine(int muxLine) {
    if (this->muxPin == -1) {
        return; // No muxing, do nothing
    }
    if (muxLine != 0 && muxLine != 1) {
        return; // Invalid muxLine, do nothing
    }
    digitalWrite(this->muxPin, muxLine);
}

/**
 * Begin transmission to the I2C device at the specified address.
 * @param muxLine The multiplexer line to use (0 or 1).
 * @param address The I2C address of the device.
 * @return Status code of the transmission.
 */
void WireManager::beginTransmission(int muxLine, int address) {
    this->switchMuxLine(muxLine);
    this->wire->beginTransmission(address);
}

/**
 * End transmission to the I2C device at the specified address.
 * @param muxLine The multiplexer line to use (0 or 1).
 * @param stop Whether to send a stop message (release the bus).
 * @return Status code of the transmission.
 */
int WireManager::endTransmission(int muxLine, bool stop) {
    this->switchMuxLine(muxLine);
    return this->wire->endTransmission(stop);
}

/**
 * Request data from the I2C device at the specified address.
 * @param muxLine The multiplexer line to use (0 or 1).
 * @param address The address of the register to read from.
 * @param quantity The number of bytes to read.
 * @param stop Whether to send a stop message (release the bus).
 * @return Status code of the transmission.
 */
int WireManager::requestFrom(int muxLine, int address, int quantity, int stop) {
    this->switchMuxLine(muxLine);
    return this->wire->requestFrom(address, quantity, stop);
}

/**
 * Check how many bytes of data are available to read from the I2C device.
 * @param muxLine The multiplexer line to use (0 or 1).
 * @return The number of bytes available to read.
 */
int WireManager::available(int muxLine) {
    this->switchMuxLine(muxLine);
    return this->wire->available();
}

/**
 * Write a value to the I2C device.
 * @param muxLine The multiplexer line to use (0 or 1).
 * @param value The value to write.
 * @return Status code of the write operation.
 */
int WireManager::write(int muxLine, int value) {
    this->switchMuxLine(muxLine);
    return this->wire->write(value);
}

/**
 * Read a byte from the I2C device.
 * @param muxLine The multiplexer line to use (0 or 1).
 */
uint8_t WireManager::read(int muxLine) {
    this->switchMuxLine(muxLine);
    return this->wire->read();
}