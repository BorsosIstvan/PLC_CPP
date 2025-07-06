#ifndef TON_H
#define TON_H

#include <chrono>

class TON
{
private:
    bool IN = false;
    bool Q = false;
    uint32_t PT = 1000;
    uint32_t ET = 0;

    std::chrono::steady_clock::time_point startTime;
    bool timing = false;
public:
    // Instellen
    void setPT(uint32_t milliseconds) { PT = milliseconds; }
    void setIN(bool input);

    // Ophalen
    bool getQ() const { return Q; }
    uint32_t getET() const { return ET; }

    // Update functie – elke loop aanroepen
    void update();

};
#endif