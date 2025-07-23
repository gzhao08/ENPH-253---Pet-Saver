#pragma once

#include <PID_v1.h>
#include <Arduino.h> 
#include <Wifi.h>
#include "driver/ledc.h"
#include "IRArray.h" 

// PINS
#define Lread 26
#define Rread 25
#define leftpwmChannel 1
#define rightpwmChannel 2
#define leftpwmOut 13
#define rightpwmOut 12

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=0, Ki=0, Kd=0; 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);   
int lastErr = 0;
const int SPEED = 800;

int displayIndex = 0;

// Wifi
WiFiServer server(80); 
const char* AP_ssid = "ESP32_Group9";
const char* AP_pass = "roborobo";

void get_params_wifi();
float get_error();
void drive_motors(double error, double PIDvalue);

void setup()
{
  Serial.begin(115200);
  //initialize the variables we're linked to
  Input = get_error();
  Setpoint = 0.0; // 0 error
  get_params_wifi();
  // reconfigure PID
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetOutputLimits(-300, 300);
  //pwm setup
  ledcSetup(leftpwmChannel, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(leftpwmOut,leftpwmChannel);

  ledcSetup(rightpwmChannel, 100,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(rightpwmOut,rightpwmChannel);

  // pinMode(Lread, INPUT);
  // pinMode(Rread, INPUT);


  // Reflectance_Array()



  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  
}

void loop()
{
  if (displayIndex == 100) {
    Serial.printf("PID Output: %lf\n", Output);
    displayIndex = 0;
  }
  Input = get_error();   
  myPID.Compute();
  drive_motors(Input,Output);  
  displayIndex++;
}


void get_params_wifi() {
  //Serial.begin(115200);
  // Start the soft AP
  WiFi.softAP(AP_ssid, AP_pass);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("AP started. Connect to %s:%s, IP=%s\n", AP_ssid, AP_pass, ip.toString().c_str());

  server.begin();

  WiFiClient client;
  boolean collected = false;

  while (!collected) {
    // try to form connection with client
    client = server.available();
    while (client && !collected) {
      Serial.println("Client found");
      while (client.connected() && !collected) {
        //Serial.println("Waiting for client to be available");
        if (client.available()) {
          Serial.printf("Client connected\n");
          String line = client.readStringUntil('\n');
          Serial.printf("Received: %s\n", line.c_str());


          if (line.startsWith("GET ")) {
            client.stop();            // drop the OS probe
            break;                    // go back to server.available()
          }

          client.println(line); // echo back to matlab
          Serial.println("Sent!");

          // Extract Values
          switch (line.charAt(0)) {
              case 'P':
                Kp = (double) line.substring(1).toInt();
                break;
              case 'D':
                Kd = (double) line.substring(1).toInt();
                break;
              case 's':
                collected = true;
                break;
          }   
        }
      }
    }
    delay(200);
  }
  Serial.printf("kp: %lf - kd: %lf\n", Kp, Kd);
  client.stop();
  Serial.println("Client disconnected");
  delay(2000);
}

float get_error() {
  Serial.begin(115200);
  int error;
  int left = digitalRead(Lread);
  int right = digitalRead(Rread);

  

  if (left&&right) error = 0;
  if (left&&!right) error = -1;
  if (!left&&right) error = +1;
  //history cases for when both sensors are off the line
  if (!left&&!right)
  {
    if (lastErr>0) error = 5;
    if (lastErr<=0) error=-5;
  }
  Serial.printf("%d %d %d\n", left, right, error);
  lastErr = error;
  return error;
}

void drive_motors(double error, double PIDvalue) {
  int leftMotorSpeed = SPEED /**(1-abs(error-lastErr)/5)*/ + PIDvalue;
  int rightMotorSpeed = SPEED /**(1-abs(error-lastErr)/5)*/ - PIDvalue;
  
// The motor speed should not exceed the max PWM value
  
  ledcWrite(leftpwmChannel,   constrain(leftMotorSpeed, 0, 4096));  
  ledcWrite(rightpwmChannel,   constrain(rightMotorSpeed, 0, 4096));
}