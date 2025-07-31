#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SectionManager.h"

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

void stopDrive();
void startDrive();

void objectDetected(void *parameter) {

  Serial.begin(115200);
  while (!Serial);
  // Initialize I2C
  // Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)
  
  Wire.begin(15, 13);  // SDA, SCL for right lidar
  Wire1.begin(7, 5);  // SDA, SCL for
  

  // DISPLAY
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  // Initialize display
  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    delay(100);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Initializing VL53L0X...");
  display.display();


  // LIDAR
  Adafruit_VL53L0X rightLidar = Adafruit_VL53L0X();
  Adafruit_VL53L0X leftLidar = Adafruit_VL53L0X();
  VL53L0X_RangingMeasurementData_t rightMeasure;
  VL53L0X_RangingMeasurementData_t leftMeasure;
  

  Serial.println("VL53L0X test");

  Serial.println("Initializing Right VL53L0X...");
  while (!rightLidar.begin( (uint8_t) 41U, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot Right VL53L0X, retrying...");
    delay(100);
  }
//   rightLidar.setAddress(0x30);
//   Wire.endTransmission();
  Serial.println("Right VL53L0X Initialized!");

  Serial.println("Initializing Left VL53L0X...");
  while (!leftLidar.begin((uint8_t) 41U, false, &Wire1, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot Left VL53L0X, retrying...");
    delay(100);
  }
//   Wire1.endTransmission();
  Serial.println("Left VL53L0X Initialized!");

  Serial.println("Both VL53L0X ready!");

  display.clearDisplay();
  display.setTextSize(3);

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }


  SectionManager sectionManager;
  
  drive = true; // start driving
  int doorwayCounter = 0;
  int thresholds[6] = {250, 250, 250, 350, 325, 325}; // thresholds for sections 0-5

  while (true){
    while (startRead) {
        Serial.println("Taking measurements");
        // Wire.beginTransmission(0x30);
        rightLidar.rangingTest(&rightMeasure, false);
        // Wire.endTransmission();
        // Wire1.beginTransmission(0x29);
        leftLidar.rangingTest(&leftMeasure, false);
        // Wire.endTransmission();
        // Wire.beginTransmission(0x3C);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println();
   

        if (rightMeasure.RangeStatus != 4) {  // 4 means invalid measurement
        
        bool onRamp = sectionManager.getCurrentSection() == 5;

        if (sectionManager.detect(rightMeasure.RangeMilliMeter, thresholds[sectionManager.getCurrentSection()], !onRamp)) {
          stopDrive();
          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(35, 16);
          display.print("STOPPED");
          display.display();
          display.clearDisplay();
          display.setTextSize(3);
          display.setCursor(0, 0);
          delay(2000);
          startDrive();
          if (sectionManager.getCurrentSection() != 5) {
            while (!sectionManager.detectOutOfRange(rightMeasure.RangeMilliMeter)) {
              rightLidar.rangingTest(&rightMeasure, false);
              delay(10);
            }
          }
          
        }
        display.setTextSize(2);
        display.setCursor(80, 24);
        display.print(rightMeasure.RangeMilliMeter);
        // display.display();
        Serial.printf("Right: %d\n", rightMeasure.RangeMilliMeter);
      
      }
      
        if (leftMeasure.RangeStatus != 4) {  // 4 means invalid measurement
            display.setTextSize(2);
            display.setCursor(0, 24);
            display.print(leftMeasure.RangeMilliMeter);
            Serial.printf("Left: %d\n", leftMeasure.RangeMilliMeter);
        }

        display.setCursor(0, 0);
        display.print(sectionManager.getCurrentSection());
        // Wire.beginTransmission(0x3C);
        display.display();
      
    }
    Serial.println("Waiting for startRead to be true");
    delay(100);
    
  }
}

void stopDrive() {
    // portENTER_CRITICAL(&mux);
    drive = false;
    // portEXIT_CRITICAL(&mux);
}

void startDrive() {
    // portENTER_CRITICAL(&mux);
    drive = true;
    // portEXIT_CRITICAL(&mux);
}



