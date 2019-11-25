#include "modbusdriver.h"
#include "log.h"



//----- ModbusDriver -----

ModbusDriver::ModbusDriver() :
    AbstractDriver(),
    m_mb(nullptr),
    m_strIp(DEFAULT_IP),
    m_port(DEFAULT_PORT)
{
    AbstractDriver::s_names.insert(name());

    m_data.regs.resize(DEFAULT_NUMBER_OF_REGISTER);
    m_data.addr = DEFAULT_START_ADDR;

    LOG("default constructor")
}


ModbusDriver::ModbusDriver(const ModbusDriver &driver) :
    AbstractDriver(driver)
{
    LOG("constructor of copy")
}


ModbusDriver::ModbusDriver(ModbusDriver &&driver) :
    AbstractDriver (static_cast<AbstractDriver&&>(driver))
{
    LOG("move constructor")
}


/*!
 * \brief Reading a single range of registers
 * \param[in] addr - start address
 * \param[in] num - number of registers
 * \return
 */
const IDriver::Data ModbusDriver::readFromSrv(uint16_t addr, uint16_t num)
{
    Data data;
    data.time = m_data.time;    // Coping last timestamp
    std::fill(data.regs.begin(), data.regs.end(), 9999);

    uint16_t tab_reg[/*num*/ MAX_BUF_SIZE];
    assert(num < MAX_BUF_SIZE);
    int numOfRegs = modbus_read_registers(m_mb, addr, num, tab_reg);
    if (-1 != numOfRegs) {
        data.time = std::chrono::system_clock::now();
        data.regs.assign(tab_reg, tab_reg + num);
        data.quality = IDriver::Data::Quality::Good;
    }
    else {
        cout << "[ModbusDriver::readFromSrv] : error=" << modbus_strerror(errno) << endl;
        data.quality = IDriver::Data::Quality::BadParameter;
        // Если данные считаны нормально хотябы для одного диапаздона то считаем что коннест есть.
        // Иначе пробуем переподключиться
        bool toReconnect = true;
        for (auto data: m_datas) {
            toReconnect &= (IDriver::Data::Quality::Good != data.quality);
        }
        cout << "[ModbusDriver::readFromSrv] : toReconnect=" << toReconnect << endl;
        if (toReconnect) {
            m_connectionState = IDriver::Connection::Disconnected;
//			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    return data;
}


/*!
 * \brief writing a single register
 * \param[in] addr - address of register
 * \param[in] value - value for writing
 * \return The method shall return true if successful
 */
bool ModbusDriver::writeToSrv(uint16_t addr, uint16_t value)
{
    // Checking for connection
    if (IDriver::Connection::Connected != m_connectionState) {
        cout << "[ModbusDriver::writeToSrv] : error: not connected" << endl;
        return false;
    }

    int numOfRegs = modbus_write_register(m_mb, addr, value);
    if (1 != numOfRegs) {
        cout << "[ModbusDriver::writeToSrv] : error: operations is failed" << endl;
        return false;
    }

    LOG("writed register: addr=%d, value=%d", addr, value);

    return true;
}


/*!
 * \brief Request of writing registers range
 * \param[in] addr - address of first register;
 * \param values - registers range.
 * \return The method shall return true if successful
 */
bool ModbusDriver::writeToSrv(uint16_t addr, const std::vector<uint16_t> &values)
{
    // Checking for connection
    if (IDriver::Connection::Connected != m_connectionState) {
        LOG("error: not connected")
        return false;
    }

    uint16_t src[128];  //< output buffer (128 - maximum size of ASDU data in Modbus/TCP)
    assert(values.size() < 128);
    for (int i=0; i<static_cast<int>(values.size()); i++)
        src[i] = values.at(static_cast<std::vector<uint16_t>::size_type>(i));
    int nb = static_cast<int>(values.size());

    int numOfRegs = modbus_write_registers(m_mb, addr, nb, src);
    if (nb != numOfRegs) {
        LOG("error: operations is failed")
        return false;
    }

    return true;
}


/*!
 * \brief Connect to server
 * \return Connection state
 */
IDriver::Connection ModbusDriver::connect()
{
    // Creation of connection
    if (nullptr == m_mb) {
        m_mb = modbus_new_tcp(m_strIp.c_str(), m_port);
        LOG("connecting to server %s:%d", m_strIp, m_port);
        if (nullptr == m_mb) {
            LOG("Unable to allocate libmodbus context")
            return IDriver::Connection::Disconnected;
        }
        int rc = modbus_set_slave(m_mb, 1);
        if (-1 == rc) {
            cerr << "Invalid slave ID"  << endl;
        }
    }

    // Connecting
    if (-1 == modbus_connect(m_mb)) {
        LOG("connection failed")
        disconnect();
        std::this_thread::sleep_for(ch::milliseconds(m_config.connectDelay));
    }
    else {
        m_connectionState = IDriver::Connection::Connected;
    }

    return m_connectionState;
}


/*!
 * \brief Disconnect from server
 * \return The method shall return current connection state
 * \bug this dangerous place <delete> (ponder)
 */
IDriver::Connection ModbusDriver::disconnect()
{
//    cout << "[ModbusDriver::disconnect]";
    LOG()
    try {
        if (nullptr != m_mb) {
            assert(nullptr != m_mb);
            modbus_close(m_mb);
            modbus_free(m_mb);
            m_connectionState = IDriver::Connection::Disconnected;
            std::lock_guard<std::mutex> lock(m_mux);
            assert(nullptr != m_mb);
//            delete m_mb; // This makes chash when 2 mb servers is disconnected
            m_mb = nullptr;
        }
        else {
            LOG("driver have been disconnected already");
        }
    }
    catch (...) {
        cout << "[ModbusDriver::disconnect] exceptiong" << endl;
    }

    return m_connectionState;
}


/*!
 * \brief Checks input communication config string for validation
 *
 * This string must have following format: "ipAddress[:port]"
 * id - is optional parameter
 * \example ("192.168.1.46" - 192.168.1.46:502);
 * \example "192.168.1.46:10502" - 192.168.1.46:10502;
 * \param[in] strConfig - input config string
 * \return true - valid
 */
bool ModbusDriver::isComConfigValid(const string &strConfig)
{
    // "ipaddress:port"
    std::regex rgx("^"
                   "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                   "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                   "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                   "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\:"
                   "([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?"
                   "$");

    return std::regex_match(strConfig, rgx);
}


/*!
 * \brief Checks input data config string for validation
 *
 * This string must have following format: "{startAddr, num}"
 * \example "{0,10}" - 10 registers from 0 address.
 * \example "{0,10} {10,22}" - 10 registers (addr=0) and 22 registers (addr=10)
 * \param[in] strConfig - input config string
 * \return true - valid
 */
bool ModbusDriver::isDataConfigValid(const string &strConfig)
{
    LOG();
//    std::regex rgx("^"
//                   " *[0-9]{1,5}, *[0-9]{1,3} *"
//                   "$");
    regex rgx("^"
             "( *\\{\\d{1,5}, *(12[0-8]|1[0-9][0-9]|[01]?[0-9][0-9]?) *\\})+ *"
             "$");
    return std::regex_match(strConfig, rgx);
}


/*!
 * \brief Extract ip address and port number
 * \return true - OK:
 * \bug calculation of port insn't work
 */
bool ModbusDriver::writeComConfig()
{
    // Checking for bad configuration
    if (!isComConfigValid(m_config.comConfig)) {
        return false;
    }

    string strIp;
    string strPort;

    try {
        strIp = m_config.comConfig.substr(0, m_config.comConfig.find(':'));
        strPort = m_config.comConfig.substr(m_config.comConfig.find(':')+1, string::npos);
        m_strIp = strIp;
        m_port = std::stoi(strPort);
    }
    catch (std::invalid_argument &e) {
        cout << "[ModbusDriver::writeComConfig] exception: " << e.what() << endl;
        m_port = DEFAULT_PORT;
    }
    catch (...) {
        cout << "Default exception" << endl;
        m_port = DEFAULT_PORT;
    }
    return true;
}


/*!
 * \brief Parser of data configuration
 *
 * Parse configuration string and extract:
 * + registers range
 *  - first register addres
 *  - number of registers
 * \return true - OK
 * \bug Doesn't work properly. ({0,10}{10,3} - exception atoi)
 */
bool ModbusDriver::writeDataConfig()
{
//    cout << "[ModbusDriver::writeDataConfig]" << endl;
    LOG();
    // Checking for bad configuration
    if (!isDataConfigValid(m_config.dataConfig)) {
        return false;
    }

    string startAddr;
    string numReg;
    m_datas.clear();

    try {
        string m1("{");
        string m2("}");
        vector<string> strs = Utilites::substrByToken(m_config.dataConfig, m1, m2);
        for (string str: strs) {
            Data data;
            startAddr = str.substr(0, str.find(','));
            data.addr = std::stoi(startAddr);
            numReg = str.substr(m_config.dataConfig.find(','), string::npos);
            int num = std::stoi(numReg);
            data.regs.resize(num);
            m_datas.push_back(data);
        }
    }
    catch (std::exception &e) {
        cout << "[ModbusDriver::writeComConfig] exception: " << e.what() << endl;
        m_data.addr = DEFAULT_START_ADDR;
        m_data.regs.resize(DEFAULT_NUMBER_OF_REGISTER);
    }
    catch (...) {
        cout << "exception: " << endl;
        m_data.addr = DEFAULT_START_ADDR;
        m_data.regs.resize(DEFAULT_NUMBER_OF_REGISTER);
    }

    return true;
}




