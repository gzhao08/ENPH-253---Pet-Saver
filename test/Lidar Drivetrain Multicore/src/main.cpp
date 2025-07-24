#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lidar.h"
#include "drivetrain.h"

// Task function for core 0
void task1(void *pvParameters);
     
// Task function for core 1
void task2(void *pvParameters);
     
void app_main() {
// Create task for core 0
xTaskCreatePinnedToCore(task1, "Task1", 2048, NULL, 5, NULL, 0);
     
// Create task for core 1
xTaskCreatePinnedToCore(task2, "Task2", 2048, NULL, 5, NULL, 1);
}