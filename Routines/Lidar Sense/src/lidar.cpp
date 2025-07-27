#include <Arduino.h>
#include <lidar.h>
#include <Adafruit_VL53L0X.h>
#include "../GlobalConstants.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1



void objectDetected(void *parameter) {

  // Initialize I2C
  Wire.begin(15, 13);  // Wire.begin(sda-21, scl-22)

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
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  int lastmeasure = 0;
  int section = 0;
  int num_consecutive = 0; // number of consecutive measurements below threshold
  int thresholds[2] = {DOORWAY_THRESH, RAMP_THRESH}; // thresholds for each section

  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X test");

  

  while (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    delay(100);
  }

  //lox.setMeasurementTimingBudgetMicroSeconds(20000);
  Serial.println("VL53L0X ready!");


  display.clearDisplay();
  display.setTextSize(3);
  display.print("Test");
  display.display();
  delay(2000);
  display.clearDisplay();

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }

  while (true){
    while (startRead) {
      
      VL53L0X_RangingMeasurementData_t measure;

      // unsigned long currentMillis = millis();
      lox.rangingTest(&measure, false); // pass in 'true' to get debug data
      // Serial.println("Benchmark: " + String(millis() - currentMillis) + " ms");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println();

      if (measure.RangeStatus == 0 && measure.RangeMilliMeter !=8191) {  // 4 means out of range

        switch (section) {

          case 0: {
            // Before doorway
            if(measure.RangeMilliMeter<=DOORWAY_THRESH){
              num_consecutive++;
              if (num_consecutive >= 5) { // require 5 measurements for doorway detection
                portENTER_CRITICAL(&mux);
                drive = false;
                portEXIT_CRITICAL(&mux);
                ++section %= 4;
                num_consecutive = 0;
              }
              
            }
            else {
              num_consecutive = 0;
            }
            break;

          }
          case 1: {
            // Before ramp
            if(measure.RangeMilliMeter<=RAMP_THRESH){
              num_consecutive++;
              if (num_consecutive >= 5) { // if we have 3 consecutive measurements below the threshold
                portENTER_CRITICAL(&mux);
                drive = false;
                portEXIT_CRITICAL(&mux);
                ++section %= 4;
                num_consecutive = 0;
              }
            }
            else {
              num_consecutive = 0;
            } 
            break;
          }
          case 2: {
            // after sensing beginning of ramp; while on ramp
            if(measure.RangeMilliMeter>RAMP_THRESH){
              num_consecutive++;
              if (num_consecutive >= 10) { // require 10 consecutive measurements to be on ramp
                portENTER_CRITICAL(&mux);
                drive = false;
                portEXIT_CRITICAL(&mux);
                ++section %= 4;
                num_consecutive = 0;
              }
            }
            else {
              num_consecutive = 0;
            } 
            break;
          }
          
          case 3: {
            // after ramp
            if(measure.RangeMilliMeter>RAMP_THRESH){
              num_consecutive++;
              if (num_consecutive >= 10) { // require 10 consecutive measurements to be off ramp
                portENTER_CRITICAL(&mux);
                drive = false;
                portEXIT_CRITICAL(&mux);
                ++section %= 4;
                num_consecutive = 0;
              }
            }
            else {
              num_consecutive = 0;
            } 
            break;
          }
        }
      }
      display.print(section);
      display.display();
      Serial.println(measure.RangeMilliMeter);
      
    }
    Serial.println("Waiting for startRead to be true");
    delay(100);
    
  }
}