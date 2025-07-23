#include <Arduino.h>

#define ERR_4 7
#define ERR_3 5
#define ERR_2 3
#define ERR_1 1

class IRArray {
    private:
    
        int pin1, pin2, pin3, pin4; // left to right
        int r1,r2,r3,r4; // corresponding pin readings
        int error = 0, lastError = 0;
    public:

        IRArray(int outerLeftPin, int innerLeftPin, int innerRightPin, int outerRightPin) {
            pin1 = outerLeftPin;
            pin2 = innerLeftPin;
            pin3 = innerRightPin;
            pin4 = outerRightPin;

            pinMode(pin1, INPUT);
            pinMode(pin2, INPUT);
            pinMode(pin3, INPUT);
            pinMode(pin4, INPUT);
        }

        void takeReading(boolean show) {
            r1 = digitalRead(pin1);
            r2 = digitalRead(pin2);
            r3 = digitalRead(pin3);
            r4 = digitalRead(pin4);
            if (show) {
                showReading();
            }
        }

        void showReading() {
            Serial.printf("1: %d, 2: %d, 3: %d, 4: %d\n", r1, r2, r3, r4);
        }

        void showState() {
            showReading();
            Serial.printf("Error: %d, Last Error: %d\n", error, lastError);
        }

        int getError() {

            if (r2&&r3) error = 0; // on the line

            else if (r1||r2) { // on left side
                if (!r1 && r2) { // inner left
                    error = -ERR_1;            
                }
                else if (r1 && r2) { // mid left
                    error = -ERR_2;
                }
                else if (r1 && !r2) { // outer left
                    error = -ERR_3;
                }
            }

            else if (r3||r4) { // on right side
                if (!r4 && r3) { // inner right
                    error = ERR_1;            
                }
                else if (r4 && r3) { // mid right
                    error = ERR_2;
                }
                else if (r4 && !r3) { // outer right
                    error = ERR_3;
                }
            }

            else if (!r1&&!r2&&!r3&&!r4) { // totally off line
                if (lastError < 0) { // on the left
                    error = -ERR_4;
                }

                else {
                    error = ERR_4;
                }   
            }

            return error;
        }

        void update() {
            lastError = error;
            return;
        }
        
};