#include <WiFi.h>

//––––––––––––––––––––
// Configuration
const char* AP_ssid = "ESP32_Group9";
const char* AP_pass = "roborobo";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Start the soft AP
  WiFi.softAP(AP_ssid, AP_pass);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("AP started. Connect to %s:%s, IP=%s\n", AP_ssid, AP_pass, ip.toString().c_str());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;
  Serial.println("Client Connected!");
  // Simple echo server
  while (client.connected()) {
    if (client.available()) {

      String line = client.readStringUntil('\n');
      Serial.printf("Received: %s\n", line.c_str());
      client.print("Echo: ");
      client.println(line);
    }
  }
  client.stop();
  Serial.println("Client disconnected");
}
