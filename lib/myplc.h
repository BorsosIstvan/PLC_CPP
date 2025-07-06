#ifndef MY_PLC_H
#define MY_PLC_H

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

class MyPLC {
private:
    std::array<bool, 64> IX{};
    std::array<bool, 64> QX{};
    std::array<bool, 64> M{};
    std::array<uint16_t, 16> IW{};
    std::array<uint16_t, 16> QW{};

public:
    bool getIX(int index) const;
    void setIX(int index, bool value);

    bool getQX(int index) const;
    void setQX(int index, bool value);

    bool getM(int index) const;
    void setM(int index, bool value);

    uint16_t getIW(int index) const;
    void setIW(int index, uint16_t value);

    uint16_t getQW(int index) const;
    void setQW(int index, uint16_t value);

    void printStatus() const;
    void loop(); // Simuleert één PLC-cyclus
};

#endif
