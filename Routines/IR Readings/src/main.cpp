#include <Arduino.h>
#include <WiFi.h>
//#include "../sensors/IRArray.h"  
#include "SteeringManager.h" 
#include "../GlobalConstants.h"


IRArray array;

void setup() {
  array.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN);
  Serial.begin(115200);  
}

void loop() {
  array.takeReading(false);
  array.getError();
  array.showState();
  Serial.print("\n");
  array.update();
}