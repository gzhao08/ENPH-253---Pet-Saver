#include <Wire.h>
#include <Adafruit_LIS3MDL.h>

#define NUM_TRIALS 200
#define THRESH 2

float last_reading = 0.0;

Adafruit_LIS3MDL lis3mdl;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("LIS3MDL test");

  //Wire.begin(25, 26);  // defaults to GPIO21 (SDA), GPIO22 (SCL) on ESP32

  if (!lis3mdl.begin_I2C()) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) delay(10);
  }

  Serial.println("LIS3MDL Found!");
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.println("ready");
}

void loop() {
  float x_tot = 0;
  float y_tot = 0;
  float z_tot = 0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    sensors_event_t event;
    lis3mdl.getEvent(&event);
    x_tot += event.magnetic.x;
    y_tot += event.magnetic.y;
    z_tot += event.magnetic.z;
  }
  x_tot /= NUM_TRIALS;
  y_tot /= NUM_TRIALS;
  z_tot /= NUM_TRIALS;
  float magnitude = sqrt(x_tot*x_tot+y_tot*y_tot+z_tot*z_tot);

  

  //Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print(" uT, ");
  //Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print(" uT, ");
  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.println(" uT");
  //Serial.printf("%f,%f,%f,%f\n", x_tot, y_tot, z_tot,magnitude);
  if (abs(last_reading - z_tot) > THRESH) {
    Serial.println("unstable Reading, no field detected");
  }
  else {
    Serial.printf("%f\n",z_tot);
  }
  last_reading = z_tot;

  delay(10);
}
