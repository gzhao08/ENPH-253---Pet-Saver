#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SectionManager.h"
#include "SharedState.h"

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

  Serial.println("Right VL53L0X Initialized!");

  Serial.println("Initializing Left VL53L0X...");
  while (!leftLidar.begin((uint8_t) 41U, false, &Wire1, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot Left VL53L0X, retrying...");
    delay(100);
  }

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
  int thresholds[6] = {250, 250, 250, 350, 325, 350}; // thresholds for sections 0-5
  int stops[6] = {false, false, true, true, true, true};
  int useRightLidar[6] = {true, true, true, true, true, false};
  int consecutiveCount[] = {2, 2, 2, 2, 15, 2}; // number of consecutive measurements to consider a section change
  int objectCount = 0;

  while (true){
    while (startRead) {
        // take measurements
        rightLidar.rangingTest(&rightMeasure, false);
        leftLidar.rangingTest(&leftMeasure, false);

        // setup display
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println();
        
        bool onRamp = sectionManager.getCurrentSection() == 5;

        if (rightMeasure.RangeStatus != 4) {  // 4 means invalid measurement
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

        if ((useRightLidar[objectCount] ? rightMeasure.RangeStatus != 4 : leftMeasure.RangeStatus != 4) && sectionManager.detect((useRightLidar[objectCount] ? rightMeasure.RangeMilliMeter : leftMeasure.RangeMilliMeter), thresholds[objectCount], true, consecutiveCount[objectCount])) {
            stopDrive();
            if (stops[objectCount]) {
                display.clearDisplay();
                display.setTextSize(2);
                display.setCursor(35, 16);
                display.print("STOPPED");
                display.display();
                display.clearDisplay();
                display.setTextSize(3);
                display.setCursor(0, 0);
                delay(2000);
            }
            startDrive();

            // sense break
            if (useRightLidar[objectCount]) {
                while (!sectionManager.detectOutOfRange(rightMeasure.RangeMilliMeter)) {
                    rightLidar.rangingTest(&rightMeasure, false);
                    delay(10);
                }
            }
            else {
                while (!sectionManager.detectOutOfRange(leftMeasure.RangeMilliMeter)) {
                    leftLidar.rangingTest(&leftMeasure, false);
                    delay(10);
                }
            }
            objectCount++;
        }

        display.setCursor(0, 0);
        display.print(objectCount);
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



