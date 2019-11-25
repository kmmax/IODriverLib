#ifndef IDRIVER_H
#define IDRIVER_H

#include <string>
#include <chrono>
#include <vector>
#include <set>

/*!
 * \brief External interface of Driver.
 *
 * \todo
 * - add method for reading registers by address. Will think about using this
 * functionality as different class (adaptor);
 * - add method for writing group of registers.
 * - mark methods as const if it need
 * - create method comStateToString
 * - make printData static
 */
class IDriver
{
public:
    /// Exchange data
    struct Data {
        enum class Quality {
            Undef = 0,  		///< Undefined
            Good  = 1,          ///< Good
            NotRun = 2,         ///< Driver doesn't run
            NoConnect = 3,      ///< Not connection to remote device
            BadParameter = 4    ///< Remote device doesn't have parameter
        };

        std::chrono::system_clock::time_point time; ///< group registers timestamp
        uint16_t addr = 0;                          ///< address of first register
        std::vector<uint16_t> regs;                 ///< registers array
        Quality quality = Quality::Undef;           ///< quality

        std::string quality_to_string(Data::Quality quality) {
            switch (quality) {
            case Data::Quality::Good:
                return std::string("Good");
                break;
            case Data::Quality::NoConnect:
                return std::string("Err conn");
                break;
            case Data::Quality::NotRun:
                return std::string("Not run");
                break;
            case Data::Quality::BadParameter:
                return std::string("Bad param");
                break;
            default:
                return std::string("Undef");
                break;
            }
        }
    };

    /// Configuration status
    enum class Config {
        Undef,                              ///< Parameter is undefined
        Ok,                                 ///< Parameter valid
        BadParameter                        ///< Parameter is invalid
    };

    /// Driver state
    enum class DriverState {
        Undef,                              ///< State is undefined
        Run,                                ///< The driver performs a cyclic reading.
        Stop                                ///< Driver performs no work
    };

    /// Remote device connection state
    enum class Connection {
        Undef,                              ///< Connection is undefined
        Connected,                          ///< Connection is established
        Disconnected,                       ///< No connection
        Connecting,                         ///< Connecting in process
        Disconnecting                       ///< Disconnection in process
    };

    /// Driver's name
    virtual std::string name() const = 0;
    /// Driver instanse ID
    virtual int id() = 0;
    /// Returns names supported drivers
    virtual std::set<std::string> names() const = 0;

    /// Writes single register
    virtual bool write(uint16_t addr, uint16_t value) = 0;
    /// Writes registers range
    virtual bool write(uint16_t addr, std::vector<uint16_t> values) = 0;
    /// Read registers
    virtual Data read() const = 0;
    /// Returns all range of registers
    virtual const std::vector<Data>& readDatas() =0;
    /// Returns the range (s) of registers with the given address value
    virtual const std::vector<Data>& readDatas(uint16_t addr) = 0;

    /// Sets communication parameters
    virtual Config setComConfigure(const std::string &strConfig) = 0;
    /// Sets data configuration
    virtual Config setDataConfigure(const std::string &strConfig) = 0;
    /// Adds new data regions
    virtual Config addDataConfigure(std::string &strRegion) = 0;
    /// Returns a configuration settings
    virtual const std::string& comConfigure() = 0;
    /// Regurns a configuration of data
    virtual const std::string& dataConfigure() = 0;
    /// Sets delay between requests, ms
    virtual int setRequestDelay(int delay) = 0;
    /// Sets between trying of connections, ms
    virtual int setConnectDelay(int delay) = 0;
    /// Runs driver
    virtual bool start() = 0;
    /// Stops driver
    virtual bool stop() = 0;

    /// Returns of driver's state
    virtual DriverState driverState() = 0;
    /// Returns connection states to remote device
    virtual Connection connectionState() = 0;

    /// Prints context of data buffer. This debug method
    virtual void printData(const IDriver::Data &data) = 0;
    /// Presents Data::Quality as string
    virtual std::string qalityAsString(Data::Quality quality) = 0;
    /// Presets Time Point as strign
    virtual const std::string timeAsString(const std::chrono::system_clock::time_point tp) const = 0;

};

#endif // IDRIVER_H
