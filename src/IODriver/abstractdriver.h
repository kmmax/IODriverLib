

#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

// Common headers
#include "idriver.h"

// This for std::min; std::max (msvs2015 compiler show mistake)
// Name conflict windows.h
//#define NOMINMAX


// Personal headers
#include "utilites.h"
#include "log.h"

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

using std::cout;
using std::endl;




/*!
 *  This class is used for time control during logging
 */
template <typename duration = ch::seconds, typename clock = ch::high_resolution_clock>
class timer
{
    typename clock::time_point m_start, m_stop;

    typename clock::rep get_time() const
    {
        return ch::duration_cast<duration>(m_stop - m_start).count();
    }

public:
    void         start() { m_start = clock::now(); }
    const timer& stop()  { m_stop = clock::now(); return *this; }

    std::ostream& print() const
    {
#ifdef  LOG_ENABLE
        int span = get_time();
        return std::cout << "[" << std::setw(8) << span << "] : ";
#else
        return std::cout;
#endif
    }
};


/*!
  * \class AbstractDriver
 *  \brief Asynchronous modbus client
 *
 * This class is common implementation of driver's interface (\ref IDriver)
 *
 *  It performs following tasks:
 *  - creates separate thread and runs worker method inside.
 *  - controls state of driver.
 *  - starts dispatcher (\ref manager) which controls requests and commands of writing.
 *
 * \todo
 * - to realize thread-safe logging
 * - to realize thread-safe transfering data when reading
 * - to create PLC simulator (libmodbus interface)
 * - to create several ranges for reading
 * - to make m_data atomic
 * - set ID device using data configuration
 * - to split file into sepatated files (separated class)
 * - to make more effective working with m_data (by reference)
 *
 * \bug
 * - don't show true quality when many data regions
 */
class  AbstractDriver : public IDriver
{
public:
    explicit AbstractDriver();
    virtual ~AbstractDriver();

    AbstractDriver(const AbstractDriver& driver);
    AbstractDriver(AbstractDriver&& driver);

    // IDriver interface
    Data read() const override;
    const std::vector<Data> &readDatas() override;
    const std::vector<Data> &readDatas(uint16_t addr) override;
    bool write(uint16_t addr, uint16_t value) override;
    bool write(uint16_t addr, std::vector<uint16_t> values) override;
    std::set<string> names() const override;
    bool start() override;
    bool stop() override;
    Config setComConfigure(const std::string &strConfig) override;
    Config setDataConfigure(const std::string &strConfig) override;
    Config addDataConfigure(string &strRegion) override;
    const std::string &comConfigure() override { return  m_config.comConfig; }
    const std::string &dataConfigure() override { return m_config.dataConfig; }
    int setRequestDelay(int delay) override;
    int setConnectDelay(int delay) override;
    DriverState driverState() override { return m_driverState; }
    Connection connectionState() override { return  m_connectionState; }

    static const string driverStateToString(const IDriver::DriverState state);
    static const string connectiionStateToString(const IDriver::Connection state);
    static std::set<std::string> drvNames();

    void printData(const IDriver::Data &data) final;
    string qalityAsString(Data::Quality quality) final;
    const string timeAsString(const system_clock::time_point tp) const override;

protected:
    // Constants
    const int DEFAULT_REQUEST_DELAY = 1000; ///< Default value of delay between requests, ms
    const int DEFAULT_CONNECT_DELAY = 3000; ///< Default value of delay between connections, ms

    /// Internal commands for driver's manager
    enum class Cmd {
        No = 0,     ///< There are no commands
        Write = 1   ///< There is command for writing
    };

    /// Internal representation of configuration
    struct Config
    {
        int requestDelay;       ///< Dellay between requests, ms
        int connectDelay;       ///< Dellay between trying of connections, ms
        std::string comConfig;  ///< String of communication configuration
        std::string dataConfig; ///< String of data configuration
    } m_config;

    // Parameters
    IDriver::DriverState m_driverState;
    IDriver::Connection m_connectionState;

    // Data
    Data m_data;                            ///< Internal data: buffer
    vector<Data> m_datas;                   ///< Internal data: all ranges
    vector<Data> m_datasProxy;              ///< Internal data: selected ranges
    static set<std::string> s_names;        ///< Names of all drivers

    // Multithread
    std::thread *m_thread1;                 ///< Work thread
    void worker();                          ///< Worker for thread
    std::atomic_bool m_done;                ///< the flag for thread that driver has finished its work
    std::atomic_bool m_runFlag;             ///< the flag: driver is run or not
    std::mutex m_mux;                       ///< Mutex
    std::condition_variable m_condVar;      ///< \todo to recall what did it need.

    // Commands for write
    Cmd cmd;                                ///< Command to drive control
    std::atomic<uint16_t> addrToWrite;      ///< Address of register for write
    std::atomic<uint16_t> valueToWrite;     ///< Value of register for write
    std::vector<uint16_t> valuesToWrite;    ///< Block of data to write \todo make atomic if possible (ponder)

    // Logging
    static std::map<IDriver::DriverState, std::string> m_driverStateMap;
    static std::map<IDriver::Connection, std::string> m_connectionStateMap;
    timer<ch::milliseconds> m_timer;        ///< Time tick for logging

protected:
    // Methods for overloading
    virtual const Data readFromSrv(uint16_t addr, uint16_t num) = 0;                    ///< Single read of registers range
    virtual bool writeToSrv(uint16_t addr, uint16_t value) = 0;                         ///< Single write one register
    virtual bool writeToSrv(uint16_t addr, const std::vector<uint16_t> &values) = 0;    ///< Single write of registers range
    virtual IDriver::Connection connect() = 0;                          				///< Connection to remote device
    virtual IDriver::Connection disconnect() = 0;                       				///< Disconnects from remote device
    virtual bool isComConfigValid(const std::string &strConfig) = 0;		    		///< Checks communication configure string
    virtual bool isDataConfigValid(const std::string &strConfig) = 0;		    		///< Checks data configure string
    virtual bool writeComConfig();
    virtual bool writeDataConfig();

private:
    void manager();             						///< Dispatcher of requests
    int nextIndex();   									///< Return next data (cyclic) from datas buffer
    std::vector<Data>::size_type m_dataIndex = 0;       ///< index of current data (from datas buffer)
    void safeDataCopy(const Data& in, Data& output); 	///< Thread safe function of copy data buffer

};



#endif // ABSTRACTDRIVER_H
