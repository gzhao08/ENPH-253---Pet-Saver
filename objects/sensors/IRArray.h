#pragma once

#include <Arduino.h>

#define ERR_4 7
#define ERR_3 5
#define ERR_2 3
#define ERR_1 1

class IRArray {
    private:
    
        int pin1, pin2, pin3, pin4; // left to right
        int r1,r2,r3,r4; // corresponding pin readings
        int error, lastError;
    public:

        IRArray();

        void begin(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin);

        void takeReading(boolean show);
        void showReading();
        void showState();
        int getError();
        void update();
        
};