#include "ton.h"
using namespace std::chrono;

void TON::setIN(bool input){
    if (input && !IN)
    {
        // Start timer
        startTime = std::chrono::steady_clock::now();
        timing = true;
    }else if (!input)
    {
        // Reset
        timing = false;
        ET = 0;
        Q = false;
    }
    IN = input;
}

void TON::update(){
    if ( timing){
        ET = duration_cast<milliseconds>(steady_clock::now() - startTime).count();

        if (ET >= PT){
            Q = true;
            ET = PT; // niet hoger dan preset tonen
        }
    }
}