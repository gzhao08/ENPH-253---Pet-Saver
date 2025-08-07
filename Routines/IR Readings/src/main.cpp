#include <Arduino.h>
#include <WiFi.h>
//#include "../sensors/IRArray.h"  
#include "SteeringManager.h" 
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LIS3MDL.h>
#include <VL53L0X.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1


IRArray array;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

void setup() {
  array.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN);
  Serial.begin(115200);  

  // DISPLAY
  Wire1.begin(15, 13);  // SDA = GPIO15, SCL = GPIO13

  
  // Initialize display
  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    delay(100);
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Start");
  display.display();
  delay(500);
}

void loop() {
  char* readings = new char[7]; // Allocate memory for readings
  display.clearDisplay();
  display.setCursor(0, 0);
  array.takeReading(false);
  array.getError();
  array.showState();
  Serial.print("\n");
  array.getReadings(readings);
  Serial.println(readings);
  display.print(readings);
  display.display();
  array.update();
}