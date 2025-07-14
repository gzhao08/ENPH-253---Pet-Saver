// reads an analog signal on pin 36 (ADC1_CHANNEL_0)
// writes a pwm signal to pin 4 (with LED and series resistor connected from LAB 0) whos duty cycle is proportional to the read analog value


#include <Arduino.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/adc.h"


#define pwmChannel1 1
#define pwmChannel2 2
#define pwmOut1 7
#define pwmOut2 5


#define potThreshold 2048


// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define PIN_BUTTON 12


// #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int pot_read = 0;


void setup() {
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_12); // ADC1_CHANNEL_0 = pin 36
  ledcSetup(pwmChannel1,1000,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcSetup(pwmChannel2,1000,12);
  ledcAttachPin(pwmOut1, pwmChannel1);
  ledcAttachPin(pwmOut2,pwmChannel2);


  // display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  // display_handler.clearDisplay();
}


void loop() {
  pot_read = adc1_get_raw(ADC1_CHANNEL_0); // reads the analog value of ADC1_CHANNEL_0 (pin 36) -- between 0 and 4095
  int pot_value = pot_read;
  if (pot_read > 2048) {
    ledcWrite(pwmChannel1,pot_read);
    ledcWrite(pwmChannel2, 0);
  }
  else {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, 1024);
  }
 
  // writes a dutycycle to the specified pwmchannel (which in this case was linked to pin 4)
 


  // display_handler.clearDisplay();
  // display_handler.setTextSize(1);
  // display_handler.setTextColor(SSD1306_WHITE);
  // display_handler.setCursor(0,0);
  // display_handler.println("PWM:" + pot_value);
  // display_handler.display();


  delay(10); // brief delay of 10ms
}
