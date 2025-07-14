#include <Arduino.h>
#include "driver/ledc.h"

#define Lread 26
#define Rread 25
#define kpread 38
#define kdread 35 
#define leftpwmChannel 1
#define rightpwmChannel 2
#define leftpwmOut 13
#define rightpwmOut 12

//constants
int thresh = 800;
int kp = 0;
int kd = 0;

//updating variables
int error = 0;
int left = 0;
int right = 0;
int lasterr = 0;
int speed = 0; 

//counters
int runcounter= 1;
int run = 0;
int displayloop = 0;

//control variables
int p = 0;
int d = 0;
int con = 0;

void motor(int PIDvalue);

void setup() {
//read proportional/derivative gain 
  kp = analogRead(kpread);
  kd = analogRead(kdread);

//pwm setup
  ledcSetup(leftpwmChannel,100,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(leftpwmOut,leftpwmChannel);

  ledcSetup(rightpwmChannel,100,12); // (pwmchannel to use,  frequency in Hz, number of bits)
  ledcAttachPin(rightpwmOut,rightpwmChannel);

  Serial.begin(115200);
 }


void loop() {
  
//read sensors
  left = analogRead(Lread);
  right = analogRead(Rread);

  

  if ((left>thresh)&&(right>thresh)) error = 0;
  if ((left>thresh)&&(right<thresh)) error = -1;
  if ((left<thresh)&&(right>thresh)) error = +1;
//history cases for when both sensors are off the line
  if ((left<thresh)&&(right<thresh))
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
  con = p+d;

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
  lasterr=error;
}

void motor(int PIDvalue)
{
  speed = ((4096)-abs(error-lasterr)*819.2)*.2;
  
  int leftMotorSpeed = speed + PIDvalue;
  int rightMotorSpeed = speed - PIDvalue;
  
// The motor speed should not exceed the max PWM value
  constrain(leftMotorSpeed, 409.6, 4096); 
  constrain(rightMotorSpeed, 409.6, 4096);
  
  ledcWrite(leftpwmChannel,leftMotorSpeed);  
  ledcWrite(rightpwmChannel,rightMotorSpeed);

}