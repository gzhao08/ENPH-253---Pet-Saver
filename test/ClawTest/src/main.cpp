#include <Arduino.h>
#include <Wire.h>

#include "claw/ClawTester.h"

ClawTester clawTester;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(3000);
  // 1. Initialize Wire (I2C-SDA, I2C_SCL) -- clock next to dot then data
  Wire.begin(I2C_SDA_B_PIN, I2C_SCL_B_PIN);
  Wire1.begin(I2C_SDA_A_PIN, I2C_SCL_A_PIN);

  clawTester.use();
}

void loop() {
  
}