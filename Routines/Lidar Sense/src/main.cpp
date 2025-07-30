#include <Arduino.h>
#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_task_wdt.h"

#include "lidar.h"
#include "drivetrain.h"

volatile boolean drive = true;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

volatile boolean startRead = false; // used to start reading the lidar data (no need to use locking)

void setup() {
    esp_task_wdt_delete(NULL);
    // Create task for core 0
    xTaskCreatePinnedToCore(driveTrain, "MotorTask", 4096, NULL, 5, NULL, 0);
    
    // Create task for core 1
    xTaskCreatePinnedToCore(objectDetected, "LidarTask", 4096, NULL, 5, NULL, 1);
}

void loop() {};