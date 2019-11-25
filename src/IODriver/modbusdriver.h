

#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

// Common headers
#include "abstractdriver.h"


// This for std::min; std::max (msvs2015 compiler show mistake)
// Name conflict windows.h
// #define NOMINMAX		// Uncommit for msvs2015


// Personal headers
#include "utilites.h"

#include "modbus.h"
#include "modbus-tcp.h"

#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <cassert>
#include <regex>

#include <cstring>
#include <iomanip>  // for io manipulaters like setw


using std::cout;
using std::endl;
using std::string;

namespace ch = std::chrono;


/*!
 * \brief The ModbusDriver class
 */
class ModbusDriver : public AbstractDriver
{
public:
    explicit ModbusDriver();
    explicit ModbusDriver(const ModbusDriver& driver);
    explicit ModbusDriver(ModbusDriver&& driver);

    // IDriver interface
    string name() const override { return std::string("Modbus"); }
    int id() override { return 0; }

    // Class for testing
    friend class ModbusDriverTester;

protected:
    // AbstractDriver interface
    const Data readFromSrv(uint16_t addr, uint16_t num) override;
    bool writeToSrv(uint16_t addr, uint16_t value) override;
    bool writeToSrv(uint16_t addr, const std::vector<uint16_t> &values) override;
    IDriver::Connection connect() override;
    IDriver::Connection disconnect() override;
    bool isComConfigValid(const std::string &strConfig) override;
    bool isDataConfigValid(const string &strConfig) override;
    bool writeComConfig() override;
    bool writeDataConfig() override;

private:
    // default definitions
    const std::string DEFAULT_IP = "127.0.0.1";     //< ip
    const int DEFAULT_PORT = 502;					//< port
    static const uint16_t MAX_BUF_SIZE = 128;       //< buffer size for libmodbus
    const uint16_t DEFAULT_START_ADDR = 0;          //< first register address
    const uint16_t DEFAULT_NUMBER_OF_REGISTER = 10; //< number of register

    modbus_t *m_mb;     //< libmodbus device
    string m_strIp;     //< tcp/ip address
    int m_port;         //< tcp/ip port
};


/*!
 * \brief Class for unit-testing
 */
class ModbusDriverTester
{
public:
    ModbusDriverTester() {}
    bool isDataConfigValid(std::string &strConfig) {
        ModbusDriver drv;
        return drv.isDataConfigValid(strConfig);
    }
};

#endif // MODBUSDRIVER_H
