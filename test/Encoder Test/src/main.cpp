#include <Arduino.h>
#include "driver/ledc.h"
#include <Adafruit_AS5600.h>

#define pwmChannel1 1
#define pwmChannel2 2
#define pwmOut1 5
#define pwmOut2 7 
#define TARGET_ANGLE 10

Adafruit_AS5600 as5600;

int frequency = 2000;
int duty_cycle = 3072;
boolean done = false;
int counts_left = 0;

boolean reached_position(int current_count, int target_count);

void setup() {
  // pwm initialization
  /*
  pinMode(motor, OUTPUT);
  */

  ledcSetup(pwmChannel1,1000,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcSetup(pwmChannel2,1000,12);
  ledcAttachPin(pwmOut1, pwmChannel1);
  ledcAttachPin(pwmOut2,pwmChannel2); 
  
  Serial.begin(115200);
  Serial.println("test");

  // as5600 initialization
  if (!as5600.begin()) {
    Serial.println("Could not find AS5600 sensor, check wiring!");
    while (1)
      delay(10);
  }

  Serial.println("AS5600 found!");


  as5600.enableWatchdog(false);
  // Normal (high) power mode
  as5600.setPowerMode(AS5600_POWER_MODE_NOM);
  // No Hysteresis
  as5600.setHysteresis(AS5600_HYSTERESIS_OFF);

  // analog output
  // as5600.setOutputStage(AS5600_OUTPUT_STAGE_ANALOG_FULL);

  // OR can do pwm!
  as5600.setOutputStage(AS5600_OUTPUT_STAGE_DIGITAL_PWM);
  as5600.setPWMFreq(AS5600_PWM_FREQ_920HZ);

  // setup filters
  as5600.setSlowFilter(AS5600_SLOW_FILTER_16X);
  as5600.setFastFilterThresh(AS5600_FAST_FILTER_THRESH_SLOW_ONLY);

  Serial.println("Waiting for magnet detection...");

  while (! as5600.isMagnetDetected());

  Serial.println("Magnet detected!");

  // Reset position settings to defaults
  while (!as5600.setZPosition(0));
  while (!as5600.setMPosition(4095));
  while (!as5600.setMaxAngle(4095));
  
}

void loop() {
  if (!done) {
    int target_count = round(TARGET_ANGLE * 11.37777);
    
    int initial_count = as5600.getAngle();
    Serial.printf("Initial count: %d\n", initial_count);

    target_count = (initial_count + target_count)%4095;
    Serial.printf("Target count: %d\n", target_count);

    
    // drive the motor slower (at a lower duty cycle) as it approached the position (at full power (12V) the motor is almost 60 degrees off)
    // therefore we can only drive the motor at full power if the desired angle is greater than 60 degrees (probably only if a lot bigger)
    // using a formula like duty_cycle = min(counts_left*5,4096)
    while (!reached_position(as5600.getAngle(), target_count)) {
      // Serial.printf("Count: %d\n", as5600.getAngle());
      duty_cycle = 900;
      
      //min(counts_left*7/2,4096);
      ledcWrite(pwmChannel1, duty_cycle); 
      ledcWrite(pwmChannel2, 0);
    }
    //ledcWrite(pwmChannel1,0);
    //ledcWrite(pwmChannel2, 0);   
    Serial.println("Position reached!");
    delay(500);
    Serial.printf("Actual stop count: %d\n", as5600.getAngle());

    double stop_count = 0;
    for (int i = 0; i < 50; i++) {
      stop_count+=as5600.getAngle();
    }
    stop_count/=50;

    double total_steps;

    if (stop_count<initial_count) {
      total_steps = 4096-initial_count+stop_count;
    }

    else {
      total_steps = stop_count-initial_count;
    }

    Serial.printf("Total steps: %lf, Actual stop angle: %lf\n", total_steps, round(total_steps*360/4096));
    done = true;
  }
}

boolean reached_position(int current_count, int target_count) {
  // a +/- 0.5 degree tolerance means around +/- 6 encoder steps

    if (target_count<current_count) {
      counts_left = 4096-current_count+target_count;
    }

    else {
      counts_left = target_count-current_count;
    }

  if (abs(current_count - target_count) <= 6 ) {
    return true;
  }

  return false;
}



// lets say i am at some encoder count "a" and want to go "b" encoder steps to reach a desired position and i know that the motion of the motor  
// causes the encoder counts to increase. then i can calculate the stop count with (a+b)%4096. the stop angle can be calculated with
// if (stop_count<initial_count): steps = 4096-initial_count+stop_count
// else steps = stop_count - initial_count
