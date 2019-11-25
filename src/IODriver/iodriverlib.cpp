#include "modbusdriver.h"
#include "memorydriver.h"
#include "version.h"

#include "iodriverlib.h"

#include <sstream>


IODriverLib::IODriverLib() :
    m_names{"Modbus", "Memory"}
{
    m_drv = new ModbusDriver();
    m_drv->setRequestDelay(200);
    std::string config("127.0.0.1:10502");
    m_drv->setComConfigure(config);
}




/*!
 * \brief Returns full names list of supported drivers
 * \return names list
 */
std::vector<string> IODriverLib::driverNames() const
{
    std::set<std::string> input = AbstractDriver::drvNames();
    std::vector<std::string> output;
    std::copy(input.begin(), input.end(), std::back_inserter(output));
    return output;
}


/*!
 * \brief Version of library
 *
 * Fornmat: v.MAJOR.MINOR.PATCH
 * Example: v.0.0.1
 * \return
 */
string IODriverLib::version() const
{
    string version("v."+std::to_string(DRIVER_VER_MAJOR)+"."
                   + std::to_string(DRIVER_VER_MINOR)+"."
                   + std::to_string(DRIVER_VER_BUILD)+" "
                   + "(patch " + std::to_string(DRIVER_VER_PATCH) + ")");
    return version;
}


/*!
 * \brief Creates driver instance by name
  \return pointer of driver otherwise nullptr
 * \todo creation of driver must be smart pointer (ponder)
 */
IDriver *IODriverLib::createDriver(const string name, const string options)
{
    assert(m_names.size() == 2);

    IDriver *drv = nullptr;
    // Modbus
    if (m_names.at(0) == name) {
        drv = new ModbusDriver();
        if (!options.empty()) {
            std::string strConf = options;
            drv->setComConfigure(strConf);
        }
    }
    // Memory
    else if (m_names.at(1) == name) {
        drv = new MemoryDriver();
        m_drivers.push_back(drv);
    }
    else {
        drv = nullptr;
    }

    m_drivers.push_back(drv);
    return drv;
}


/*!
 * \brief Returns info about driver
 *
 * - name and version
 * \return info string
 */
string IODriverLib::info() const
{
    using std::stringstream;

    stringstream ss;
    ss << NAME << " " << this->version() << endl;

    return ss.str();
}

/*!
 * \brief Shows info about existed drivers
 * \return infos about drivers
 */
std::vector<IODriverLib::DriversInfo> IODriverLib::driversInfo() const
{
    std::vector<IODriverLib::DriversInfo> infos;
    return infos;
}









































