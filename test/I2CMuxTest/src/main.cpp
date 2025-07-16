#include <Arduino.h>
#include <Wire.h>

#define AS5600_ADDR 0x36

// Digital control pins connected to CD4053 select lines
#define SEL_A  14  // Connect to A
#define SEL_B  27  // Connect to B

void setMuxChannel(uint8_t ch) {
  // Up to 4 channels using 2 control bits
  digitalWrite(SEL_A, ch & 0x01);        // LSB
  digitalWrite(SEL_B, (ch >> 1) & 0x01); // MSB
  delay(2);  // Allow time for signal to settle
}

uint16_t readRawAngle() {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0C);  // RAW ANGLE MSB
  if (Wire.endTransmission(false) != 0) {
    return 0xFFFF;
  }

  Wire.requestFrom(AS5600_ADDR, 2);
  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    return ((uint16_t)msb << 8) | lsb;
  }

  return 0xFFFF;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(15, 13);

  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  Serial.println("CD4053BE Mux Test for AS5600");
}

void loop() {

    uint16_t angle = readRawAngle();
    if (angle != 0xFFFF) {
      float deg = (angle * 360.0) / 4096.0;

      Serial.print(deg, 2);
      Serial.println(" deg");
    } else {
      Serial.println(": No AS5600 detected.");
    }


  Serial.println("-----");
  delay(500);
}