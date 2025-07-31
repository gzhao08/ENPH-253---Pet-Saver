#pragma once

#include <Arduino.h>
#include "../sensors/IRArray.h"
#include <PID_v1.h>
#include "../motors/DCMotor.h" 
#include "SharedState.h" 
 
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
        void backward(int duty, int timeInMS);
        void stop();
        void quickStop();
        void turnAround(int duty, boolean clockwise);
        void lineFollow(int baseSpeed);

        // Tune PID
        void setPID(double kp, double kd);

};