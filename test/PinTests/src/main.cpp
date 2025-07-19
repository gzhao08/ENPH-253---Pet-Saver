#include <Arduino.h>

int GPIOPins[] = {
  // 37,38,
  // 34,35,
  9,10,
  5,7,
  8,
  19,22,
  21,20,
  0,4,
  2,12,
  15,13,
  14,27,
  33,32,
  26,25
  // 39,36
};

int IOnlyPins[] = {
  37,38,
  34,35,
  39,36
};

// put function declarations here:

void setup() {
  Serial.begin(115200);
  // Strapping pins (by default they are driven high/low)
  // 0  - 1
  // 2  - 0 
  // 5  - 1 
  // 12 - 0 
  // 15 - 1

  for (int pin : IOnlyPins) {
    pinMode(pin, INPUT);
  }

  for (int pin : GPIOPins) {
    pinMode(pin, OUTPUT);
  }
  // pinMode(2, OUTPUT);
  // pinMode(5, OUTPUT);
  // pinMode(12, OUTPUT);
  // pinMode(15, OUTPUT);

  // digitalWrite(2, HIGH);
  // digitalWrite(5, LOW);
  // digitalWrite(12, HIGH);
  // digitalWrite(15, LOW);

}

void testInputOnly() {
  for (int pin : IOnlyPins) {
    if (digitalRead(pin) == 1) {
      Serial.print("Pin ");
      Serial.print(pin);
      Serial.println(" is high");
    }
  }
}

void loop() {
  // testInputOnly();

  for (int pin : GPIOPins) {
    digitalWrite(pin, HIGH);
  }

  delay(500);

  for (int pin : GPIOPins) {
    digitalWrite(pin, LOW);
  }

  delay(500);
  // Serial.println("Is this working?");
}
