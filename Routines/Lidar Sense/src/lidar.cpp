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
  VL53L0X_RangingMeasurementData_t measure;

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

  SectionManager sectionManager;

  while (!startRead) {
    Serial.println("Waiting for startRead to be true");
    delay(200); // wait for the drivetrain to set startRead to true (done PID tuning)
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println();
  display.print(0);

  drive = true; // start driving
  int doorwayCounter = 0;
  int thresholds[3] = {DOORWAY_THRESH, FIRST_PET_THRESH, RAMP_THRESH};  

  while (true){
    while (startRead) {
      
      // unsigned long currentMillis = millis();
      lox.rangingTest(&measure, false); // pass in 'true' to get debug data
      // Serial.println("Benchmark: " + String(millis() - currentMillis) + " ms");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println();
   

      if (measure.RangeStatus != 4) {  // 0 means valid measurement

        // switch (sectionManager.getCurrentSection()) {

        //   case 0: {
        //     // Before doorway
        //     if (sectionManager.detectDoorway(measure.RangeMilliMeter)) {
              
        //       if (doorwayCounter == 0) {
        //         doorwayCounter++;
        //         while (!sectionManager.detectOutOfRange(measure.RangeMilliMeter)) {
        //           lox.rangingTest(&measure, false);
        //           delay(10);
        //         }
        //       }

        //       if (doorwayCounter == 1) {
        //         display.print(sectionManager.getCurrentSection());
        //         display.display();
        //         stopDrive(); // stop the drive train
        //         delay(1000); // wait for the drive train to stop
        //         startDrive(); // start the drive train again
        //         while (!sectionManager.detectOutOfRange(measure.RangeMilliMeter)) {
        //           lox.rangingTest(&measure, false);
        //           delay(10);
        //         }
        //       }
              
        //     }
            
        //     break;

        //   }
        //   case 1: {
        //     // First pet
        //     if (sectionManager.detectFirstPet(measure.RangeMilliMeter)) {
        //       display.print(sectionManager.getCurrentSection());
        //       // display.setCursor(50, 24);
        //       // display.print(measure.RangeMilliMeter);
        //       display.display();
        //       stopDrive(); // stop the drive train
        //       delay(1000); // wait for the drive train to stop
        //       startDrive(); // start the drive train again
        //       while (!sectionManager.detectOutOfRange(measure.RangeMilliMeter)) {
        //         lox.rangingTest(&measure, false);
        //         delay(10);
        //       }
        //     }
        //     break;
        //   }
        //   case 2: {
        //     // after sensing beginning of ramp; while on ramp
        //     if (sectionManager.detectRamp(measure.RangeMilliMeter)) {
        //       display.print(sectionManager.getCurrentSection());
        //       // display.setCursor(50, 24);
        //       // display.print(measure.RangeMilliMeter);
        //       display.display();
        //       stopDrive(); // stop the drive train
        //       delay(1000); // wait for the drive train to stop
        //       startDrive(); // start the drive train again
        //     }
        //     break;
        //   }
          
        //   case 3: {
        //     // after ramp
        //     if (sectionManager.detectEndOfRamp(measure.RangeMilliMeter)) {
        //       display.print("End");
        //       display.display();
        //       stopDrive(); // stop the drive train
        //       delay(3000); // wait for the drive train to stop
        //     }
        //     break;
        //   }
        // }



        int threshold = 250;
        if (sectionManager.getCurrentSection() == 3) {
          threshold = 350;
        } else if (sectionManager.getCurrentSection() == 4 || sectionManager.getCurrentSection() == 5) {
          threshold = 325;
        }
        bool onRamp = sectionManager.getCurrentSection() == 5;


        if (sectionManager.detect(measure.RangeMilliMeter, threshold, !onRamp)) {
          stopDrive();
          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(35, 16);
          display.print("STOPPED");
          display.display();
          display.clearDisplay();
          display.setTextSize(3);
          delay(2000);
          startDrive();
          if (sectionManager.getCurrentSection() != 5) {
            while (!sectionManager.detectOutOfRange(measure.RangeMilliMeter)) {
              lox.rangingTest(&measure, false);
              delay(10);
            }
          }
          
        }
        display.print(sectionManager.getCurrentSection());
        display.setCursor(50, 24);
        display.print(measure.RangeMilliMeter);
        display.display();
      
      }
      
      // Serial.printf("%d , %d\n", measure.RangeMilliMeter, drive);
      
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