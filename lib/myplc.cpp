#include "MyPLC.h"
#include <windows.h> // voor Sleep()

bool MyPLC::getIX(int index) const { return IX[index]; }
void MyPLC::setIX(int index, bool value) { IX[index] = value; }

bool MyPLC::getQX(int index) const { return QX[index]; }
void MyPLC::setQX(int index, bool value) { QX[index] = value; }

bool MyPLC::getM(int index) const { return M[index]; }
void MyPLC::setM(int index, bool value) { M[index] = value; }

uint16_t MyPLC::getIW(int index) const { return IW[index]; }
void MyPLC::setIW(int index, uint16_t value) { IW[index] = value; }

uint16_t MyPLC::getQW(int index) const { return QW[index]; }
void MyPLC::setQW(int index, uint16_t value) { QW[index] = value; }

void MyPLC::printStatus() const {
    std::cout << "== PLC STATUS ==" << std::endl;

    std::cout << "IX: ";
    for (int i = 0; i < 8; i++) std::cout << IX[i];
    std::cout << "\nQX: ";
    for (int i = 0; i < 8; i++) std::cout << QX[i];
    std::cout << "\nM:  ";
    for (int i = 0; i < 8; i++) std::cout << M[i];
    std::cout << std::endl;

    std::cout << "IW0: " << IW[0] << "  QW0: " << QW[0] << " QW1 " << QW[1] << " QW2 " << QW[2] << std::endl;
    std::cout << "================\n" << std::endl;
}

void MyPLC::loop() {
    // Simpele logica:
    // Als IX0 aan is, zet dan QX0 aan, anders uit
    setQX(0, getIX(0));

    // Wissel M0 elke cyclus (flippen)
    setM(0, !getM(0));

    // Zet QW0 = IW0 + 1
    setQW(0, getIW(0) + 1);

    printStatus();
    Sleep(1000); // wacht 1 seconde
}