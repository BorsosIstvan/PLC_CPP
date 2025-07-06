#include "myplc.h"
#include "ton.h"
#include <ModbusRTUSlave.h>
#include <myserial.h>
#include <thread>
#include <vector>


MyPLC plc;
TON ton;
TON toff;
MySerial serial;
ModbusRTUSlave modbus;
bool isTonPhase = true;


void setup(){

    serial.begin("COM6", 9600);
    modbus.begin(&plc, &serial);
    modbus.start();

    ton.setPT(3000);
    toff.setPT(3000);
    plc.setQX(1, true);
    plc.setQX(0, true); // licht aan
}

void loop() {
    if (plc.getQX(1)){
        ton.update();
        ton.setIN(true);
        plc.setQX(0, true);
        if (ton.getQ()){
            ton.setIN(false);
            toff.setIN(true);
            plc.setQX(1,false);
        }
    }else{
        toff.update();
        toff.setIN(true);
        plc.setQX(0,false);
        if(toff.getQ()){
            toff.setIN(false);
            ton.setIN(true);
            plc.setQX(1,true);
        }
    }
    plc.printStatus();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main(){
    setup();
    while (true){
        loop();
    }
    serial.close();
    modbus.stop();
    return 0;
}