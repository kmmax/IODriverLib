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
    cout << lib->info() << endl;

    cout << "------------------- Poll -----------------" << endl;
    IDriver *drv = lib->createDriver("Memory");
//    IDriver *drv = lib->createDriver("Modbus");
    if (nullptr == drv)
        cout << "pointer is null" << endl;
    cout << "id: " << drv->id() << endl;
    cout << "name: " << drv->name() << endl;
    if (IDriver::Config::Ok != drv->setDataConfigure("{0,10}")) {
        cout << "Bad data config" << endl;
        return 1;
    }
    drv->setRequestDelay(500);
    drv->start();
    cout << "Begin:" << endl;
    for (int i=0; i<5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        try {
            IDriver::Data data = drv->read();
            cout << "[addr=" << data.addr << ", number=" << data.regs.size() << "] : ";
            for (uint16_t addr=data.addr; addr<data.addr+data.regs.size(); addr++) {
                std::cout << data.regs.at(addr-data.addr) << " ";
            }
            cout << "(" << data.quality_to_string(data.quality) << ")";
        }
        catch(std::exception &e) {
            cout << e.what() << endl;
        }
        cout << endl;
    }
    drv->stop();
    cout << "That's all" << endl;

    return 0;
}

