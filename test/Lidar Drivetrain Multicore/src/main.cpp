#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lidar.h"
#include "drivetrain.h"

volatile boolean stopMotors = false;
     
void app_main() {
// Create task for core 0
xTaskCreatePinnedToCore(driveTrain, "Task1", 2048, NULL, 5, NULL, 0);
     
// Create task for core 1
xTaskCreatePinnedToCore(objectDetected, "Task2", 2048, NULL, 5, NULL, 1);
}