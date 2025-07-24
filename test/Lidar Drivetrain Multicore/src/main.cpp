#include <Arduino.h>
#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lidar.h"
#include "drivetrain.h"

volatile boolean drive = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void setup() {
// Create task for core 0
xTaskCreatePinnedToCore(driveTrain, "MotorTask", 2048, NULL, 5, NULL, 0);
     
// Create task for core 1
xTaskCreatePinnedToCore(objectDetected, "LidarTask", 2048, NULL, 5, NULL, 1);
}

void loop() {};