#include "MagneticEncoder.h"

/**
 * Object representing a magnetic encoder
 * When a magnetic encoder is fixed, its output increases when a magnet rotates counter-clockwise
 */
MagneticEncoder::MagneticEncoder(int muxLine) : SensorI2C(AS5600_ADDR, muxLine) {
}

/**
 * Sets up magnetic encoder
 * @param wireManagerObject WireManager object to use for I2C communication
 */
void MagneticEncoder::begin(WireManager* wireManagerObject) {
    this->assignWireManager(wireManagerObject);

    float readAngleResult = this->readAngle();
    if (readAngleResult < 0) {
        Serial.println("MagneticEncoder: Failed to read angle during begin()");
        return;
    } 

    this->currentAngle = readAngleResult;
    this->prevAngle = this->currentAngle;
    this->relAngle = currentAngle;
}

/**
 * Read raw angle from magnetic encoder
 * @return raw angle value from 0 to 4096 IF success
 * @return 0xFFFF IF fail
 */
uint16_t MagneticEncoder::readRawAngle() {
    this->beginTransmission();
    this->write(AS5600_MSB_REG);

    int endTransmissionStatus = this->endTransmission(false); // Repeated start
    if (endTransmissionStatus != 0) { 
        Serial.println("End transmission failed");
        Serial.println(endTransmissionStatus);
        return 0xFFFF; // Transmission failed
    }

    // Request MSB and LSB
    this->requestFrom(2, true); 

    if (this->available() == 2) {
        uint8_t msb = this->read();
        uint8_t lsb = this->read();
        return ((uint16_t) msb << 8) | lsb;
    }

    return 0xFFFF;
}

/**
 * Calculate the angle based on magnetic encoder output
 * Also updates relative angle
 * @return angle value from 0 to 360 IF success
 * @return -1 IF something went wrong
 */
float MagneticEncoder::readAngle() {
  uint16_t rawAngle = readRawAngle();
  if (rawAngle != 0xFFFF) {
    float angle = (rawAngle * 360.0) / 4096.0;
    this->incrementRelAngle(angle);
    return angle;
  } else {
    Serial.println("readAngle went wrong!");
    return -1;
  }
}

/**
 * Increments the relative angle of the encoder
 * This modifies the angles
 * Assumes angle change did not exceed 180 degrees
 * @param newAngle 0 to 360;
 */
void MagneticEncoder::incrementRelAngle(float newAngle) {
    this->currentAngle = newAngle;
    float angleDifference = this->angleDifference(this->currentAngle, this->prevAngle);
    this->relAngle += angleDifference;
    this->prevAngle = this->currentAngle;
}

/**
 * Set the current angle as home
 */
void MagneticEncoder::home() {
    float readAngleResult = this->readAngle();
    if (readAngleResult < 0) {
        Serial.println("MagneticEncoder: Failed to read angle during home()");
        return;
    }
    this->currentAngle = readAngleResult;
    this->prevAngle = this->currentAngle;
    this->relAngle = 0;
}

/**
 * Calculates the difference of toAngle and fromAngle
 * Imagine them being vectors and dif = toAngle - fromAngle
 * @param toAngle 
 * @param fromAngle
 * @return a value between -180 and 180. 
 * Positive if increasing fromAngle to toAngle causes a smaller rotation than decreasing fromAngle
 */
float MagneticEncoder::angleDifference(float toAngle, float fromAngle) {
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

/**
 * Relative angle getter
 * @return relative angle in degrees
 */
float MagneticEncoder::getRelAngle() {
    float readAngleResult = this->readAngle(); // Ensure the relative angle is updated
    if (readAngleResult < 0) {
        Serial.println("MagneticEncoder: Failed to read angle in getRelAngle()");
    }
    return this->relAngle;
}



