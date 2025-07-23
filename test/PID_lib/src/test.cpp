#include <iostream>
#include <string>

#define ERR_4 7
#define ERR_3 5
#define ERR_2 3
#define ERR_1 1

int lastError = 0;
int error = 0;
int r1, r2, r3, r4;

int getError();

int main() {
    while(true) {
        std::cin >> r1
                >> r2
                >> r3
                >> r4;
        getError();
        std::cout << "Error: " << error << "\n";
        lastError = error;
    }
    
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