#pragma once

#include <Arduino.h>
#include "IRArray.h"
#include <PID_v1.h>
#include "DCMotor.h"  
 

class SteeringManager {
    private: 
        // PID
        PID pidController;
        int PIDSampleTime = 10;
        double setpoint = 0;   
        double input = 0;      
        double output;     

        // 2) Choose your initial tuning constants
        double Kp = 2.0;
        double Ki = 5.0;
        double Kd = 1.0;

        // Motor
        DCMotor* leftMotor; 
        DCMotor* rightMotor; 

        // Line Follow
        boolean followLine = false; // should be volative global
        
        
    public:
        IRArray array; 

        SteeringManager(DCMotor* left, DCMotor* right); 

        void begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin);
        
        void forward(int duty);
        void backward(int duty);
        void lineFollow(int baseSpeed);
};