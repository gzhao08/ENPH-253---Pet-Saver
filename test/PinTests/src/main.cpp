#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
  // Strapping pins (by default they are driven high/low)
  // 0  - 1
  // 2  - 0 
  // 5  - 1 
  // 12 - 0 
  // 15 - 1

  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(15, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(15, LOW);

}

void loop() {
  // digitalWrite(2, HIGH);
  // digitalWrite(5, LOW);
  // digitalWrite(12, HIGH);
  // digitalWrite(15, LOW);
  Serial.println(fmod(110.0323, 9));

  delay(10);
  // put your main code here, to run repeatedly:
}