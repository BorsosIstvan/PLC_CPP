#ifndef TOFF_H
#define TOFF_H

#include <chrono>

class TOFF
{
private:
    /* data */
    bool IN = false;
    bool Q = false;
    uint32_t PT = 1000;
    uint32_t ET = 0;

    std::chrono::steady_clock::time_point startTime;
    bool timing = false;

public:
    void setPT(uint32_t milliseconds){PT = milliseconds;}
    void setIN(bool input);

    bool getQ(){return Q;}
    uint32_t getET(){return ET;}

    void update();
};

#endif