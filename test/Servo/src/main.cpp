#include <Arduino.h>
#include "driver/ledc.h"

#define pwmChannel1 1
#define pwmOut1 4

// 3% to 11% possible for the servo 
// 11.9% duty cycle is claw fully open
// 10.2% is horizontal
// 9.4% is fully closed

int servoFreq = 50; 
//int min = 123; // Corresponds to 0 degrees
//int max = 492; // Corresponds to 180 degrees

void writePosition(int degrees);

void setup() {
  Serial.begin(115200);

  ledcSetup(pwmChannel1, servoFreq, 12); // (pwmchannel to use, frequency in Hz, number of bits)
  ledcAttachPin(pwmOut1, pwmChannel1);
}

void loop() {   
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    int pos = line.toInt();
    Serial.printf("Moving to %d", pos);
    writePosition(pos);
  }
}

void writePosition(int degrees) {
  int position = degrees * 369 / 180 + 123; // convert angle to PWM frequency out of 4096 (12 bits)
  ledcWrite(pwmChannel1, position); 
}

void writeDutyCycle(double percent) {
  ledcWrite(pwmChannel1, int(percent * 4096));
}