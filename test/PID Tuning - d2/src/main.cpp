#include <Arduino.h>
#include "driver/ledc.h"
#include <WiFi.h>

#define Lread 26
#define Rread 25
#define leftpwmChannel 1
#define rightpwmChannel 2
#define leftpwmOut 13
#define rightpwmOut 12

//updating variables
int error = 0;
int left = 0;
int right = 0;
int lasterr = 0;
int last2err = 0;
int speed = 0; 

//counters
int runcounter= 1;
int run = 0;
int displayloop = 0;

//control variables
int p = 0;
int d = 0;
int d2 = 0;
int con = 0;


// wifi stuff
const char* AP_ssid = "ESP32_Group9";
const char* AP_pass = "roborobo";
WiFiServer server(80);


int kp = 0;
int kd = 0;
int kd2 = 0;
float speed_multiplier = 0;
boolean collected = false;


void motor(int PIDvalue);

void setup() {
  Serial.begin(115200);

  // Start the soft AP
  WiFi.softAP(AP_ssid, AP_pass);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("AP started. Connect to %s:%s, IP=%s\n", AP_ssid, AP_pass, ip.toString().c_str());

  server.begin();

  WiFiClient client;
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
                kp = line.substring(1).toInt();
                break;
              case 'D':
                kd = line.substring(1).toInt();
                break;
              case 'E':
                kd2 = line.substring(1).toInt();
                break;
              case 'd': {
                String str = line.substring(1); 
                str.trim();
                speed_multiplier = str.toFloat(); 
                break;
              }

              case 's':
                collected = true;
                break;
          }   
        }
      }
    }
    delay(200);
  }
  Serial.printf("kp: %d - kd: %d - kd2: %d - speed mult: %f", kp, kd, kd2, speed_multiplier);
  client.stop();
  Serial.println("Client disconnected");
  delay(5000);




//pwm setup
  ledcSetup(leftpwmChannel, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(leftpwmOut,leftpwmChannel);

  ledcSetup(rightpwmChannel, 100,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(rightpwmOut,rightpwmChannel);

  pinMode(Lread, INPUT);
  pinMode(Rread, INPUT);

  Serial.begin(115200);
 }


void loop() {
  
  //read sensors
  left = digitalRead(Lread);
  right = digitalRead(Rread);

  

  if (left&&right) error = 0;
  if (left&&!right) error = -1;
  if (!left&&right) error = +1;
  //history cases for when both sensors are off the line
  if (!left&&!right)
  {
    if (lasterr>0) error = 5;
    if (lasterr<=0) error=-5;
  }

  //approximating derivative
  if (!(error==lasterr))
  {
    run=runcounter;
    runcounter=1;
  }

  //pd control
  p=kp*error/10;
  d=(int)((float)kd*(float)(error-lasterr)/(float)(run+runcounter));
  d2 = kd2*(error + last2err - 2*lasterr) / (float) ((run+runcounter) * (run+runcounter));
  con = p+d-d2;

  //display print every 100
 if (displayloop==100)
  {
    Serial.printf("%d %d %d %d %d %d %d\n", right, left, kp, kd, p, d, error);

    displayloop=0;
  }
  displayloop=displayloop+1;
  runcounter=runcounter+1;

  //motor control
  motor(con);

  //history update
  last2err = lasterr;
  lasterr=error;
  
}

void motor(int PIDvalue)
{
  speed = ((4096)-abs(error-lasterr)*819.2)*speed_multiplier;
  
  int leftMotorSpeed = speed + PIDvalue;
  int rightMotorSpeed = speed - PIDvalue;
  
// The motor speed should not exceed the max PWM value
  
  ledcWrite(leftpwmChannel,  leftMotorSpeed /*constrain(leftMotorSpeed, 410, 4096)*/ );  
  ledcWrite(rightpwmChannel,  rightMotorSpeed /*constrain(rightMotorSpeed, 410, 4096)*/);

}