#include "toff.h"
using namespace std::chrono;

void TOFF::setIN(bool input){
    if(!input && IN){
        // set timer
        startTime = steady_clock::now();
        timing = true;
    }else if (input)
    {
        timing = false;
        ET = 0;
        Q = true;
        IN - true;
    }
    IN = input;
}

void TOFF::update(){
    if (timing){
        ET = duration_cast<milliseconds>(steady_clock::now() - startTime).count();
        if(ET >= PT){
            Q = false;
            ET = PT;
        }
    }
}