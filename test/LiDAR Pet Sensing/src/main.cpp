#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Arduino.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#define LED_PIN 26
#define BUFF_SIZE 250

uint16_t sweep_buffer[BUFF_SIZE];

void detect_pet();


void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X Pet Sensing");

  // Initialize I2C
  Wire.begin(21, 22);  // Wire.begin(sda, scl)

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }

  pinMode(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, HIGH);

  Serial.println("VL53L0X ready!");
}

void loop() {
  detect_pet();
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void detect_pet() {
  // scan until a pet is detected
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure);

  float avg = 0;
  uint16_t measurement_sum = 0;
  float num_measurements = 0;
  float previous_avg = 0;

  uint16_t buffer[100];

  for (int i = 0; i < 100; i++) {
      uint16_t measurement = measure.RangeMilliMeter;
      //buffer[i] = measurement;
      if (measurement != 8191) {
        measurement_sum += measurement;
        num_measurements++;
      }
  }

  previous_avg = (float) measurement_sum / num_measurements;
  avg = previous_avg;
  measurement_sum = 0;
  num_measurements = 0;

  while (true) {
    for (int i = 0; i < 100; i++) {
      uint16_t measurement = measure.RangeMilliMeter;
      if (measurement != 8191) {
        measurement_sum += measurement;
        num_measurements++;
      }
    }

    avg = (float) measurement_sum / 100.0;
    measurement_sum = 0;
    num_measurements = 0;

    Serial.printf("Avg: %f, Prev_avg: %f\n", avg, previous_avg);

    if (abs(avg - previous_avg) > 50) {
      return;
    }
    previous_avg = avg;

    
  }
}