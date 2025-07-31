#include <Arduino.h>
#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_task_wdt.h"

#include "LidarTest.h"
#include "DriveTrainTest.h"
#include "SharedState.h"

void setup() {
    drive = true;
    esp_task_wdt_delete(NULL);
    // Create task for core 0
    xTaskCreatePinnedToCore(driveTrain, "MotorTask", 4096, NULL, 5, NULL, 0);
    
    // Create task for core 1
    xTaskCreatePinnedToCore(readLidar, "LidarTask", 4096, NULL, 5, NULL, 1);
}

void loop() {};