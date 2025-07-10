#include <WiFi.h>


const char* AP_ssid = "ESP32_Group9";
const char* AP_pass = "roborobo";
WiFiServer server(80);


int kp = 0;
int kd = 0;
int duty = 0;
boolean collected = false;


void setup() {
  Serial.begin(115200);

  // Start the soft AP
  WiFi.softAP(AP_ssid, AP_pass);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("AP started. Connect to %s:%s, IP=%s\n", AP_ssid, AP_pass, ip.toString().c_str());

  server.begin();
  
  //WiFiClient client = server.available();
  //while (!client); // wait for connection
  //Serial.println("Client Connected!");

  WiFiClient client;
  while (true) {
    client = server.available();
    if (client) {
      Serial.println("Client Connected!");
      break;
    }
    delay(10);
  }

  // Obtain PID and base duty cycle values;

  // Simple echo server
  Serial.println("Collecting PD and Duty Cycle Values...");
  while (!collected) {
    while (client.connected()) {
      if (client.available()) {

        String line = client.readStringUntil('\n');
        Serial.printf("Received: %s\n", line.c_str());
        //client.print("Echo: ");
        //client.println(line);

        // Extract Values
        switch (line.charAt(0)) {
            case 'P':
              kp = line.substring(1).toInt();
              break;
            case 'D':
              kd = line.substring(1).toInt();
              break;
            case 'd':
              duty = line.substring(1).toInt();
              break;
        }   
      }
    }
    if (kp != 0 && kd != 0 && duty != 0) {
      collected = true;
    }
  }
  
  client.stop();
  Serial.println("Client disconnected");




}

void loop() {
  //Serial.println("In loop");
  //delay(1000);
}



