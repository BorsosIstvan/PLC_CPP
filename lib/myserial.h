#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <string>
#include <windows.h>

class MySerial {
public:
    bool begin(const std::string& portName, int baudRate);
    void print(const std::string& data);
    void println(const std::string& data);
    std::string read();
    bool available();
    void close();

private:
    HANDLE hSerial = INVALID_HANDLE_VALUE;
};

extern MySerial Serial;

#endif
