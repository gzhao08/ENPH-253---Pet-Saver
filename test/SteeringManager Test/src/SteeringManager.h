#pragma once

#include <Arduino.h>
#include "IRArray.h"
#include <PID_v1.h>
#include "DCMotor.h"  

extern volatile boolean drive; // boolean indicating when to stop driving ; should be global and changed via interrupts
 
class SteeringManager {
    private: 
        // PID
        PID pidController;
        int PIDSampleTime = 10;
        double setpoint = 0;   
        double input = 0;      
        double output;     

        // 2) Choose your initial tuning constants
        double Kp = 0.0;
        double Ki = 0.0;
        double Kd = 0.0;

        // Motor
        DCMotor* leftMotor; 
        DCMotor* rightMotor; 
        
    public:
        IRArray array; 

        SteeringManager(DCMotor* left, DCMotor* right); 

        void begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin);
        
        // Movement
        void forward(int duty);
        void backward(int duty);
        void stop();
        void lineFollow(int baseSpeed);

        // Tune PID
        void setKp(double Kp) { 
            this->Kp = Kp; pidController.SetTunings(Kp, Ki, Kd); 
        }
        
        void setKi(double Ki) { 
            this->Ki = Ki; pidController.SetTunings(Kp, Ki, Kd); 
        }

        void setKd(double Kd) { 
            this->Kd = Kd; pidController.SetTunings(Kp, Ki, Kd); 
        }

};