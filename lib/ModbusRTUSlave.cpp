#include "ModbusRTUSlave.h"
#include <iostream>
#include <chrono>

ModbusRTUSlave::ModbusRTUSlave() : running(false) {}

void ModbusRTUSlave::begin(MyPLC* plcRef, MySerial* serialRef) {
    plc = plcRef;
    serial = serialRef;
    inputBuffer = "";
}

void ModbusRTUSlave::start() {
    running = true;
    worker = std::thread(&ModbusRTUSlave::update, this);
}

void ModbusRTUSlave::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}

void ModbusRTUSlave::update() {
    while (running) {
        if (serial->available()) {
            std::string data = serial->read();
            for (char c : data) {
                buffer.push_back((uint8_t)c);

                if (buffer.size() >= 8) {
                    // We controleren of we genoeg data hebben (voor 0x03 of 0x04)
                    size_t frameLen = 8; // minimaal
                    if (buffer.size() >= frameLen) {
                        processFrame(buffer);
                        buffer.clear();
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void ModbusRTUSlave::processFrame(std::vector<uint8_t>& frame) {
    if (frame.size() < 8) return;

    // CRC check
    uint16_t receivedCRC = frame[frame.size() - 2] | (frame[frame.size() - 1] << 8);
    std::vector<uint8_t> dataNoCRC(frame.begin(), frame.end() - 2);
    uint16_t computedCRC = calculateCRC(dataNoCRC);

    if (receivedCRC != computedCRC) {
        serial->println("CRC error");
        return;
    }

    uint8_t slaveAddress = frame[0];
    if (slaveAddress != 1) return;

    uint8_t functionCode = frame[1];
    uint16_t startAddr = (frame[2] << 8) | frame[3];
    uint16_t quantity = (frame[4] << 8) | frame[5];

    if (quantity < 1 || quantity > 2000) return;  // Modbus max 2000 coils

    std::vector<uint8_t> response;
    response.push_back(slaveAddress);
    response.push_back(functionCode);


    if (functionCode == 0x01) {
        // 🟡 Functie 0x01: Read Coils → QX[]
        uint8_t byteCount = (quantity + 7) / 8;
        response.push_back(byteCount);

        for (uint8_t i = 0; i < byteCount; ++i) {
            uint8_t byteVal = 0;
            for (uint8_t b = 0; b < 8; ++b) {
                uint16_t bitIndex = i * 8 + b;
                if (bitIndex < quantity) {
                    if (plc->getQX(startAddr + bitIndex)) {
                        byteVal |= (1 << b); // LSB first
                    }
                }
            }
            response.push_back(byteVal);
        }
    }
    else if (functionCode == 0x03 || functionCode == 0x04) {
        // 🟢 Functie 0x03: Read Holding Registers → QW[]
        // 🟣 Functie 0x04: Read Input Registers → IW[]
        if (quantity > 16) return;

        response.push_back(quantity * 2); // byte count
        for (uint16_t i = 0; i < quantity; ++i) {
            uint16_t val = (functionCode == 0x03) ? plc->getQW(startAddr + i)
                                                  : plc->getIW(startAddr + i);
            response.push_back((val >> 8) & 0xFF);
            response.push_back(val & 0xFF);
        }
    }
    else if (functionCode == 0x05 && frame.size() >= 8) {
        // ⛳ 0x05 Write Single Coil
        uint16_t coilAddr = (frame[2] << 8) | frame[3];
        uint16_t value = (frame[4] << 8) | frame[5];

        if (value == 0xFF00)
            plc->setQX(coilAddr, true);
        else if (value == 0x0000)
            plc->setQX(coilAddr, false);
        else
            return; // Ongeldige waarde

        // Echo terug het originele frame (Modbus-specificatie)
        serial->print(std::string(frame.begin(), frame.end()));
    }
    else if (functionCode == 0x06 && frame.size() >= 8) {
        // 📝 0x06 Write Single Holding Register
        uint16_t regAddr = (frame[2] << 8) | frame[3];
        uint16_t regValue = (frame[4] << 8) | frame[5];

        plc->setQW(regAddr, regValue);

        // Echo terug het originele frame
        serial->print(std::string(frame.begin(), frame.end()));
    }

    else {
        // ❌ Unsupported function → exception response (optioneel)
        return;
    }

    uint16_t crc = calculateCRC(response);
    response.push_back(crc & 0xFF);
    response.push_back((crc >> 8) & 0xFF);

    serial->print(std::string(response.begin(), response.end()));
}


uint16_t ModbusRTUSlave::calculateCRC(const std::vector<uint8_t>& data) {
    uint16_t crc = 0xFFFF;
    for (uint8_t b : data) {
        crc ^= b;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}
