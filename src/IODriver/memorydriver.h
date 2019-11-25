#ifndef MEMORYDRIVER_H
#define MEMORYDRIVER_H

// Common headers
#include "abstractdriver.h"

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


///*!
// * \brief The AbstractDriver implementation - MemoryDriver class
// */
class MemoryDriver : public AbstractDriver
{
public:
    explicit MemoryDriver();
    explicit MemoryDriver(const MemoryDriver& driver);
    explicit MemoryDriver(MemoryDriver&& driver);

public:
    // IDriver interface
    string name() const override { return std::string("Memory");}
    int id() override { return 0;}	// This is dummy

protected:
    // AbstractDriver interface
    const Data readFromSrv(uint16_t addr, uint16_t num) override;
    bool writeToSrv(uint16_t addr, uint16_t value) override;
    bool writeToSrv(uint16_t addr, const std::vector<uint16_t> &values) override;
    IDriver::Connection connect() override;
    IDriver::Connection disconnect() override;
    bool isComConfigValid(const string &strConfig) override;
    bool isDataConfigValid(const string &strConfig) override;
    bool writeDataConfig() override;
    bool writeComConfig() override;

private:
//    IDriver::Connection m_conState;
    const uint16_t DEFAULT_START_ADDR = 0;          //< first register address
    const uint16_t DEFAULT_NUMBER_OF_REGISTER = 10; //< number of register

protected:
};




#endif // MEMORYDRIVER_H 
