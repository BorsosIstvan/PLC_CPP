#include "myserial.h"
#include <windows.h>
#include <iostream>

MySerial Serial;

bool MySerial::begin(const std::string& portName, int baudRate) {
    hSerial = CreateFileA(
        portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr, OPEN_EXISTING,
        0, nullptr
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Fout: kan " << portName << " niet openen.\n";
        return false;
    }

    // Serial instellingen
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Fout: kan COM-instellingen niet ophalen.\n";
        close();
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Fout: kan COM-instellingen niet instellen.\n";
        close();
        return false;
    }

    // Timeouts instellen
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 20;
    timeouts.ReadTotalTimeoutConstant = 20;
    timeouts.ReadTotalTimeoutMultiplier = 5;
    timeouts.WriteTotalTimeoutConstant = 20;
    timeouts.WriteTotalTimeoutMultiplier = 5;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Fout: kan timeouts niet instellen.\n";
        close();
        return false;
    }

    return true;
}

void MySerial::print(const std::string& data) {
    DWORD bytesWritten;
    WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, nullptr);
}

void MySerial::println(const std::string& data) {
    print(data + "\r\n");
}

std::string MySerial::read() {
    char ch;
    DWORD bytesRead;
    if (ReadFile(hSerial, &ch, 1, &bytesRead, nullptr) && bytesRead > 0) {
        return std::string(1, ch);
    } else {
        return "";
    }
}

bool MySerial::available() {
    COMSTAT status;
    DWORD errors;
    ClearCommError(hSerial, &errors, &status);
    return status.cbInQue > 0;
}

void MySerial::close() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}
