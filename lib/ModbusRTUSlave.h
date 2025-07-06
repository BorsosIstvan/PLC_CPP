#ifndef MODBUS_RTUSLAVE_H
#define MODBUS_RTUSLAVE_H

#include "myplc.h"
#include "myserial.h"
#include <string>

#include <thread>
#include <atomic>

#include <vector>

class ModbusRTUSlave {
public:
    ModbusRTUSlave();
    void begin(MyPLC* plcRef, MySerial* serialRef);
    void start();      // start de thread
    void stop();       // stopt de thread

private:
    MyPLC* plc = nullptr;
    MySerial* serial = nullptr;
    std::string inputBuffer;
    std::vector<uint8_t> buffer;
    std::thread worker;
    std::atomic<bool> running;

    std::uint8_t slaveID = 1;

    void update();            // deze blijft privé
    void processFrame(std::vector<uint8_t>& frame);
    uint16_t calculateCRC(const std::vector<uint8_t>& data);
};



#endif
