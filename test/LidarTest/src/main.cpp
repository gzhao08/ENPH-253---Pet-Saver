#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X right = Adafruit_VL53L0X();
Adafruit_VL53L0X left = Adafruit_VL53L0X();
int lastmeasure = 0;

void I2C_ScannerWire();
void I2C_ScannerWire1();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("VL53L0X test");

  // Initialize I2C
  Wire.begin(7, 5);  // Wire.begin(sda-21, scl-22)
  Wire1.begin(15, 13); // Wire1.begin(sda-15, scl-13)

  if (!right.begin((uint8_t) 41U, false, &Wire1, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot right");
    while (1);
  }

  right.setAddress(0x30); // Change the address to 0x29 for the right sensor

  Wire1.endTransmission(); // Ensure the I2C bus is clear

  if (!left.begin((uint8_t) 41U, false, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT)) {
    Serial.println("Failed to boot left");
    while (1);
  }

  Wire.endTransmission(); // Ensure the I2C bus is clear

  Serial.println("VL53L0X ready!");

  // I2C_ScannerWire();
  // I2C_ScannerWire1();

}

void loop() {
  VL53L0X_RangingMeasurementData_t rightMeasure;
  VL53L0X_RangingMeasurementData_t leftMeasure;

  right.rangingTest(&rightMeasure, false); // pass in 'true' to get debug data
  left.rangingTest(&leftMeasure, false); // pass in 'true' to get debug data

  Wire1.beginTransmission(0x30);
  if (rightMeasure.RangeStatus != 4) {  // 4 means out of range
    Serial.print("Right: "); Serial.println(rightMeasure.RangeMilliMeter);
  } 
  Wire1.endTransmission();

  Wire.beginTransmission(0x29);

  if (leftMeasure.RangeStatus != 4) {  // 4 means out of range
    Serial.print("Left: "); Serial.println(leftMeasure.RangeMilliMeter);
  } 
  Wire.endTransmission();

  delay(100);
}

// #include <Arduino.h>
// #include <Wire.h>

// void I2C_ScannerWire();
// void I2C_ScannerWire1();

// void setup() 
// {
//   Serial.begin(115200);

//   Wire.begin(15,13);             // default: 21, 22
//   Wire1.begin( 7, 5);     // I don't know the default pins !

//   Serial.println("---------- Scanning Wire -------------");
//   I2C_ScannerWire();

//   Serial.println("---------- Scanning Wire1 ------------");
//   I2C_ScannerWire1();
// }

// void loop() { delay(10); }

void I2C_ScannerWire()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

void I2C_ScannerWire1()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}