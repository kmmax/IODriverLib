#include "abstractdriver.h"
#include "log.h"


std::set<std::string> AbstractDriver::s_names;

// Names for driver states
std::map<IDriver::DriverState, std::string> AbstractDriver::m_driverStateMap = {
    { IDriver::DriverState::Undef, "Undef" },
    { IDriver::DriverState::Run, "Run" },
    { IDriver::DriverState::Stop, "Stop" }
};

// Names for connect states
std::map<IDriver::Connection, std::string> AbstractDriver::m_connectionStateMap = {
    { IDriver::Connection::Undef, "Undef" },
    { IDriver::Connection::Connected, "Connected" },
    { IDriver::Connection::Disconnected, "Disconnected" },
    { IDriver::Connection::Connecting, "Connecting" },
    { IDriver::Connection::Disconnecting, "Disconnecting" }
};


//----- AbstractDriver -----/


AbstractDriver::AbstractDriver() :
    m_driverState(IDriver::DriverState::Stop),
    m_connectionState(IDriver::Connection::Disconnected),
    m_thread1(nullptr),
    m_done(false),
    m_runFlag(false)
{
    LOG("Default constructor");

    m_timer.stop().print();

    // Configuration initialization
    m_config.requestDelay = DEFAULT_REQUEST_DELAY;
    m_config.connectDelay = DEFAULT_CONNECT_DELAY;

    m_thread1 = new std::thread(&AbstractDriver::worker, this);
    m_thread1->detach();
}


AbstractDriver::~AbstractDriver()
{
    delete m_thread1;
    m_done = true;
    LOG("Destructor");
}



AbstractDriver::AbstractDriver(const AbstractDriver &driver) :
    IDriver(driver)
{
    LOG("Constructor of copy");
}



AbstractDriver::AbstractDriver(AbstractDriver &&driver) :
    IDriver(static_cast<IDriver&&>(driver))
{
    LOG("Move constructor");
}



/*!
 * \brief Method starts driver work
 * \return
 */
bool AbstractDriver::start()
{
    LOG();
    try {
        m_runFlag = true;
        m_timer.start();
    } catch (std::exception &e) {
        cout << "[Exception: AbstractDriver::start()]: " << e.what() << endl;
    }
    return true;
}


/*!
 * \brief Method stops driver work
 * \return
 */
bool AbstractDriver::stop()
{
    LOG();
    m_runFlag = false;
    return true;
}


/*!
 * \brief Method saves communication configuration string
 * \param strConfig
 * \return current communication state
 */
IDriver::Config AbstractDriver::setComConfigure(const string &strConfig)
{
    if (!isComConfigValid(strConfig)) {
        return IDriver::Config::BadParameter;
    }

    m_config.comConfig = strConfig;
    writeComConfig();

    return IDriver::Config::Ok;
}


/*!
 * \brief AbstractDriver::setDataConfigure
 * \param strConfig
 * \return current data configuration
 */
IDriver::Config AbstractDriver::setDataConfigure(const string &strConfig)
{
    if (!isDataConfigValid(strConfig)) {
        return IDriver::Config::BadParameter;
    }

    m_config.dataConfig = strConfig;
    writeDataConfig();

    return IDriver::Config::Ok;
}


/*!
 * \brief Adds new regions of modbus registers
 * \param[in] strRegion - new region of registers
 * \return
 */
IDriver::Config AbstractDriver::addDataConfigure(string &strRegion)
{
    if (!isDataConfigValid(strRegion)) {
        return IDriver::Config::BadParameter;
    }

    m_config.dataConfig += strRegion;
    writeDataConfig();

    return IDriver::Config::Ok;
}


/*!
 * \brief Presents driver state as string
 * \param[in] state - Driver state value as enumeration
 * \return - Appropriate string
 */
const std::string AbstractDriver::driverStateToString(const IDriver::DriverState state)
{
    auto it = m_driverStateMap.find(state);
    if (m_driverStateMap.end() == it) {
        return string("???");
    }
    else {
        return it->second;
    }
}


/*!
 * \brief Presents connection state as string
 * \param[in] state - Connection value as enumeration
 * \return - Appropriate string
 */
const std::string AbstractDriver::connectiionStateToString(const IDriver::Connection state)
{
    auto it = m_connectionStateMap.find(state);
    if (m_connectionStateMap.end() == it) {
        return string("???");
    }
    else {
        std::string str = it->second;
        return str;
    }
}


/*!
 * \brief Returns possible driver's names
 * \return list of driver's names
 */
std::set<string> AbstractDriver::drvNames()
{
    return s_names;
}


/*!
 * \brief Print content of data buffer
 *
 * string has folowing format:
 * [time from start] [addr,size] time: val1, val2, ..., [qualilty]
 * \param[in] data for printing
 */
void AbstractDriver::printData(const Data &data)
{
    std::lock_guard<std::mutex> lock(m_mux);
    m_timer.stop().print();
    std::string strTime = Utilites::Time::timePointToString(data.time, Utilites::Time::TimeFormat::Time);
    cout <<  " [" << data.addr << ", " << data.regs.size() << "] ";
    cout << strTime << " :   ";
    for (auto item: data.regs) {
        cout << std::setw(5) << item << ", ";
    }
    cout << "[" << static_cast<int>(data.quality) << "] ";
    cout << endl;
}


/*!
 * \brief Converts quality to string
 * \param[in] quality
 * \return corresponding string
 */
string AbstractDriver::qalityAsString(IDriver::Data::Quality quality)
{
    string str;
    switch (quality) {
    case IDriver::Data::Quality::Good:
        str = "Good";
        break;
    case IDriver::Data::Quality::NoConnect:
        str = "Comm Err";
        break;
    case IDriver::Data::Quality::NotRun:
        str = "Not Run";
        break;
    case IDriver::Data::Quality::BadParameter:
        str = "Bad Param";
        break;
    default:
        str = "Undefined";
    }
    return str;
}


/*!
 * \brief Presents time (format: time_point) as string
 * \param[in] tp - time point;
 * \return corresponding string
 */
const string AbstractDriver::timeAsString(const system_clock::time_point tp) const
{
    return Utilites::Time::timePointToString(tp);
}


/*!
 * \brief Task works in separate thread
 */
void AbstractDriver::worker()
{
    LOG("worker is started");
    try {
        while (!m_done) {
            manager();
        }
    }
    catch (std::exception &e) {
        std::cout << "[Exception: worker] " << e.what() << endl;
    }
    catch (...) {
        std::cout << "[Exception: worker] " << endl;
    }
    LOG("worker is stoped");
}


/*!
 * \brief Stub
 * \return
 */
bool AbstractDriver::writeComConfig()
{
    return false;
}


/*!
 * \brief Stub
 * \return
 */
bool AbstractDriver::writeDataConfig()
{
    return false;
}


/*!
 * \brief AbstractDriver::manager
 * \todo chech range of all arrays and vectors
 */
void AbstractDriver::manager()
{
    try {
        m_driverState = m_runFlag? IDriver::DriverState::Run: IDriver::DriverState::Stop;
        // Driver in Run state
        if (m_runFlag) {
            LOG("Looop")
            if (IDriver::Connection::Connected == m_connectionState) {
                if (Cmd::Write == cmd) {
                    writeToSrv(addrToWrite, valuesToWrite);
                    cmd = Cmd::No;
                }
                else {
                    // Индекс циклически проходит по группам регистров
                    int index = nextIndex();
                    m_data = m_datas.at(index);
                   const Data& data = readFromSrv(m_data.addr, m_data.regs.size());
                   safeDataCopy(data, m_datas[index]);
                   {
                        std::unique_lock<std::mutex> lk(m_mux);
                        // Группы регистров опрашиваются без задержек, задержка между группой регистров
                        // Начало новой группы по индексу, равному 0
                        if (!index)
                            m_condVar.wait_for(lk, std::chrono::milliseconds(m_config.requestDelay), [=](){ return Cmd::Write == cmd; });
                   }
#ifdef LOG_ENABLE
                   printData(m_datas[index]);
#endif
                }
            }
            else {
                LOG("disconnected");
                for (auto &data: m_datas)
                    data.quality = IDriver::Data::Quality::NoConnect;
                cmd = Cmd::No;
                connect();
            }
        }
        // Driver in Stop state
        else {
            cmd = Cmd::No;
            if (IDriver::Connection::Connected == m_connectionState) {
                disconnect();
            }
            else {
                for (auto data: m_datas)
                    data.quality = IDriver::Data::Quality::NoConnect;
            }
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    catch(std::exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
}


/*!
 * \brief Calculates (increases) new number of current read data region.
 * \return new number
 */
int AbstractDriver::nextIndex()
{
    m_dataIndex++;

    if (m_dataIndex >= m_datas.size())
        m_dataIndex = 0;

    return m_dataIndex;
}


/*!
 * \brief It copies data by thread-safe way
 * \param[in] in - input Data
 * \param output - output Data
 */
void AbstractDriver::safeDataCopy(const IDriver::Data &in, IDriver::Data &output)
{
    std::lock_guard<std::mutex> lock(m_mux);
    const unsigned int minSize =  std::min(in.regs.size(), output.regs.size());
    std::copy_n(in.regs.begin(), minSize, output.regs.begin());
    output.quality = in.quality;
    output.time = in.time;
}


/*!
 * \brief Returns single data range
 * \return Data
 * \todo maybe to use mutex or something like (ponder)
 */
IDriver::Data AbstractDriver::read() const
{
    return AbstractDriver::m_data;
}


/*!
 * \brief Returns data ranges
 * \return
 */
const std::vector<IDriver::Data>& AbstractDriver::readDatas()
{
    return AbstractDriver::m_datas;
}


/*!
 * \brief Return data range(s) with the given address
 * \param[in] addr - address;
 * \return
 */
const std::vector<IDriver::Data> &AbstractDriver::readDatas(uint16_t addr)
{
    m_datasProxy.clear();
    for (auto data: m_datas) {
        if (data.addr <= addr && addr < data.addr+data.regs.size()) {
            m_datasProxy.push_back(data);
        }
    }
    return m_datasProxy;
}


/*!
 * \brief Writes single register
 * \param[in] addr - address
 * \param[in] value - value for writing
 * \return true - success, false - fail
 */
bool AbstractDriver::write(uint16_t addr, uint16_t value)
{
    std::vector<uint16_t> values{value};

    return write(addr, values);
}


/*!
 * \brief Writes registers range
 * \param addr
 * \param values
 * \return
 */
bool AbstractDriver::write(uint16_t addr, std::vector<uint16_t> values)
{
    m_timer.stop().print();

    std::lock_guard<std::mutex> lock(m_mux);
    addrToWrite = addr;
    valuesToWrite.clear();
    valuesToWrite.resize(values.size());
    const unsigned int minSize =  std::min(values.size(), valuesToWrite.size());
    std::copy_n(values.begin(), minSize, valuesToWrite.begin());

    cmd = Cmd::Write;
    m_condVar.notify_all();

    return true;
}


/*!
 * \brief Returns names of supported drivers
 * \return
 */
std::set<string> AbstractDriver::names() const
{
    return AbstractDriver::s_names;
}


/*!
 * \brief Sets delay between requests
 * \param[in] delay - delay in ms
 * \return new value of delay
 */
int AbstractDriver::setRequestDelay(int delay)
{
    m_config.requestDelay = delay;
    return m_config.requestDelay;
}


/*!
 * \brief Sets delay between trying of connection
 * \param[in] delay - delay in ms
 * \return new value of delay
 */
int AbstractDriver::setConnectDelay(int delay)
{
    m_config.connectDelay = delay;
    return m_config.connectDelay;
}

