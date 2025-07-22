#include <Arduino.h>
#include "driver/ledc.h"

#define pwmChannel 1
#define pwmOut 33
#define pwmResolution 12
#define pwmFrequency 50 // 50 Hz for servo

int minCount = 205;   // 1 ms/20 ms × 4096 = 205 --- 1 ms is full backwards
int stopCount = 307;  // 1.5/20 × 4096 = 307 --- 1.5 ms is stop
int maxCount = 410;   //2 ms/20 ms × 4096 = 410 --- 1 ms is full forwards

void setSpeed(int speed);

void setup() {
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(pwmOut,pwmChannel);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    if (line != "\n") {
      int speed = line.toInt();
      //speed = constrain(speed,-100,100);
      // int speed = constrain(line.toInt(),0,4095);
      Serial.printf("Setting speed to %d\n", speed);    
      setSpeed(speed);
    }
  }
  delay(50);
}

void setSpeed(int speed /*in percent*/) {
  int duty;
  if (speed == 0) {
    duty = 307;
  }
  else {
    duty = round(0.4076*speed + 307.3);
  }
  ledcWrite(pwmChannel, duty);
  //  ledcWrite(pwmChannel, speed);
}