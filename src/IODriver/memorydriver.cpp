#include "memorydriver.h"


//----- MemoryDriver -----

MemoryDriver::MemoryDriver()
{
    // std::fill(m_values.begin(),m_values.end(), 999);
}


MemoryDriver::MemoryDriver(const MemoryDriver &driver)
{
    Utilites::ignore(driver);
}


MemoryDriver::MemoryDriver(MemoryDriver &&driver)
{
    Utilites::ignore(driver);
}


/*!
 * \brief This method reads data from memory (source_reg)
 * \note this method is under construction
 * \param addr
 * \param num
 * \return
 * \todo make soruce register's data
 */
const IDriver::Data MemoryDriver::readFromSrv(uint16_t addr, uint16_t num)
{
    LOG()
    Data data;
    data.time = m_data.time;    // Coping last timestamp
    std::fill(data.regs.begin(), data.regs.end(), 9999);
    std::vector<uint16_t> source_reg(200);
    std::fill(source_reg.begin(), source_reg.end(), 10);

    uint16_t tab_reg[100];
    tab_reg[0] = 22;
    assert(num < MAX_BUF_SIZE);
//    int numOfRegs = modbus_read_registers(m_mb, addr, num, tab_reg);
    int numOfRegs = 10;
    num = 10;
    if (-1 != numOfRegs) {
        data.time = std::chrono::system_clock::now();
        data.regs.assign(tab_reg, tab_reg + num);
        data.quality = IDriver::Data::Quality::Good;
    }

    return data;
}


/*!
 * \brief The method write single register
 * \note This mehtod isn't realeased (dummy is used)
 * \param[in] addr - address of register
 * \param[in] value - value for writing
 * \return The method shall return true if successful
 */
bool MemoryDriver::writeToSrv(uint16_t addr, uint16_t value)
{
    // Checking for connection
    if (IDriver::Connection::Connected != m_connectionState) {
        cout << "[ModbusDriver::writeToSrv] : error: not connected" << endl;
        return false;
    }

    // Dummy!!!
    return true;
}



/*!
 * \brief Request of writing registers range
 * \note This mehtod isn't realeased (dummy is used)
 * \param[in] addr - address of first register;
 * \param values - registers range.
 * \return The method shall return true if successful
 */
bool MemoryDriver::writeToSrv(uint16_t addr, const std::vector<uint16_t> &values)
{
    // Checking for connection
    if (IDriver::Connection::Connected != m_connectionState) {
        LOG();
        return false;
    }

    // Dummy!!!
    return true;
}


/*!
 * \brief Connect to memory data range
 * \return Connection state
 */
IDriver::Connection MemoryDriver::connect()
{
    m_connectionState = IDriver::Connection::Connected;
    return m_connectionState;
}


/*!
 * \brief Disconnect from memory data range
 * \return Connection state
 */
IDriver::Connection MemoryDriver::disconnect()
{
    m_connectionState = IDriver::Connection::Disconnected;
    return m_connectionState;
}


/*!
 * \brief MemoryDriver::isComConfigValid
 * \param strConfig
 * \return
 */
bool MemoryDriver::isComConfigValid(const string &strConfig)
{
    // Dummy!!!
    return true;
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
bool MemoryDriver::isDataConfigValid(const string &strConfig)
{
    LOG();
    regex rgx("^"
             "( *\\{\\d{1,5}, *(12[0-8]|1[0-9][0-9]|[01]?[0-9][0-9]?) *\\})+ *"
             "$");
    return std::regex_match(strConfig, rgx);
}


bool MemoryDriver::writeDataConfig()
{
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


bool MemoryDriver::writeComConfig()
{
    // Dummy
    return true;
}
