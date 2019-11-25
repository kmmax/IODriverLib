#include "iodriverlib.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <exception>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    auto lib = std::make_unique<IODriverLib>();
    // IODriver info
    cout << "------------------- Info -----------------" << endl;
    cout << lib->info();

    // Poll 5 modbus registers since address=0
    cout << "------------------- Poll -----------------" << endl;
    IDriver *drv = lib->createDriver("Modbus");
    // Modbus server: ip=127.0.0.1, port=502
    drv->setComConfigure("127.0.0.1:502");
    // range №1: 0..2 (3 registers)
    // range №2: 5..12 (8 registers)
    if (IDriver::Config::Ok != drv->setDataConfigure("{0,3} {5,8}"))
        cout << "Bad data config" << endl;
    cout << "Driver: " << drv->name() << endl;
    drv->start();
    cout << "Begin:" << endl;
    for (int i=0; i<5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::vector<IDriver::Data> datas = drv->readDatas();
        std::cout << "Number of ranges: " << datas.size() << endl;
        for (auto &data: datas) {
            cout << "[addr=" << data.addr << ", number=" << data.regs.size() << "] : ";

            for (uint16_t addr=data.addr; addr<data.addr+data.regs.size(); addr++) {
                    std::cout << data.regs.at(addr-data.addr) << " ";
                }
            cout << "(" << data.quality_to_string(data.quality) << ")" << endl;
        }
        cout << endl;
    }
    drv->stop();
    cout << "That's all" << endl;

    return  0;
}
