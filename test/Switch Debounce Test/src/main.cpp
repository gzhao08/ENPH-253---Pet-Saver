#include <Arduino.h>

#define SWITCH 27
#define LED 25

int press_count = 0;
int last_press_count = 0;
boolean button_pressed = false;

void IRAM_ATTR ISR();

void setup() {
  
  attachInterrupt(SWITCH, ISR, RISING);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  Serial.begin(115200);

}

void loop() {
  if (button_pressed) {
    digitalWrite(LED,HIGH);
    delay(50);
    digitalWrite(LED,LOW);
    button_pressed = false;
  }
  if (press_count != last_press_count) {
    Serial.println(press_count);
    last_press_count = press_count;
  }
  
}

void IRAM_ATTR ISR() {
  press_count++;
  button_pressed = true;
}