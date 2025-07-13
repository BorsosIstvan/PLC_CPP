#ifndef TP_H
#define TP_H

#include <chrono>

class TP
{
private:
    bool IN = false;
    bool Q = false;
    uint32_t PT = 1000;  // Preset tijd in milliseconden
    uint32_t ET = 0;     // Elapsed Time

    std::chrono::steady_clock::time_point startTime;
    bool timing = false;

public:
    // Constructor & Destructor
    TP() = default;
    ~TP() = default;

    // Setters
    void setPT(uint32_t milliseconds) { PT = milliseconds; }
    void setIn(bool input);

    // Update de timer (moet elke cyclus aangeroepen worden)
    void update();

    // Getters
    bool getQ() const { return Q; }
    uint32_t getET() const { return ET; }
};

// ------- Functies buiten de klasse, maar inline ---------

inline void TP::setIn(bool input) {
    if (input && !IN) {
        startTime = std::chrono::steady_clock::now();
        timing = true;
        Q = true;
    } else if (!input) {
        timing = false;
        ET = 0;
        Q = false;
    }
    IN = input;
}

inline void TP::update() {
    if (timing) {
        ET = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - startTime).count();
        if (ET >= PT) {
            Q = false;
            ET = PT;
            timing = false;
        }
    }
}

#endif // TP_H
