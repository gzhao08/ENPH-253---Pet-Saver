#include <Arduino.h>
#include "../../../objects/Servo/Servo.h"

Servo* servo = new Servo(4, 1);

void setup() {
  servo->begin();
}

void loop() {
servo->writeDutyCycle(9.5);
delay(500);
servo->writeDutyCycle(10);
delay(500);
servo->writeDutyCycle(10.5);
delay(500);
servo->writeDutyCycle(11);
delay(500);
servo->writeDutyCycle(11.5);
delay(500);
servo->writeDutyCycle(12);
delay(500);

/** 
  servo->writePosition(0);
  delay(500);
  servo->writePosition(10);
  delay(500);
  servo->writePosition(20);
  delay(500);
  servo->writePosition(30);
  delay(500);
  servo->writePosition(40);
  delay(500);
  servo->writePosition(50);
  delay(500);
  servo->writePosition(60);
  delay(500);
  servo->writePosition(70);
  delay(500);
  servo->writePosition(80);
  delay(500);
  servo->writePosition(90);
  delay(500);
  servo->writePosition(100);
  delay(500);
  servo->writePosition(110);
  delay(500);
  servo->writePosition(120);
  delay(500); 
  servo->writePosition(130);
  delay(500);
  servo->writePosition(140);
  delay(500);
  servo->writePosition(150);
  delay(500);
  servo->writePosition(160);
  delay(500);
  servo->writePosition(170);
  delay(500);
  servo->writePosition(180);
  delay(500);
*/ 

}