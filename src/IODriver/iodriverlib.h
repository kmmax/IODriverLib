
#ifndef SHAREDLIB_H
#define SHAREDLIB_H

#include "idriver.h"

#include <vector>



using std::string;

/*!
 * \brief Export class of library
 * \todo Set version (for example version.h)
 *
 */
class IODriverLib
{
public:

    struct DriversInfo {
        int id;
        string name;
        IDriver::DriverState state;
        IDriver::Connection connect;
    };

    IODriverLib();

    std::vector<std::string> driverNames() const;
    std::string version() const;
    IDriver* createDriver(const std::string name, const std::string options = {});
    std::string info() const;
    std::vector<DriversInfo> driversInfo() const;

private:
    // defines
    const string NAME = "IODriver";

    IDriver *m_drv;
    const std::vector<std::string> m_names; ///< All possible names of drivers
    std::vector<IDriver*> m_drivers;
};

#endif // SHAREDLIB_H
