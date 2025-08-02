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
        double output = 0;     

        // 2) Choose your initial tuning constants
        double Kp = 0.0;
        double Ki = 0.0;
        double Kd = 0.0;

        
        
    public:
        // Motor
        DCMotor leftMotor; 
        DCMotor rightMotor; 
        IRArray array; 
        
        SteeringManager(); 

        void begin();
        
        // Movement
        void forward(int duty);
        void backward(int duty);
        void backward(int duty, int timeInMS);
        void stop();
        void quickStop();
        void turnAround(int duty, boolean clockwise);
        void turnBackwards(int duty);
        void lineFollow(int baseSpeed);
        void lineFollow();

        // Tune PID
        void setPID(double kp, double kd);

};