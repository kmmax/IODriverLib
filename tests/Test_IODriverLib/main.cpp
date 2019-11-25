// tested project
//#include "sharedlib.h"
#include "iodriverlib.h"
#include "idriver.h"

// libmodbus
//#include "modbusdriver.h"

// gtest
#include <gtest/gtest.h>

// std
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;


IODriverLib *lib = nullptr;


TEST(IODriverLib, Creation)
{
    lib = new IODriverLib();
    ASSERT_NE(nullptr, lib);
}


TEST(IODriverLib, driversNames)
{
    std::vector<string> source_names = {
        "Modbus"/*,
        "Memory"
        */
    };
    std::vector<std::string> names = lib->driverNames();

    for (auto source_name: source_names) {
        EXPECT_TRUE(names.end() != std::find(names.begin(), names.end(), source_name));
    }
}


//TEST(Modbus, DISABLE_test001)
//{
//    const string var1("192.168.1.46:10502");

//    IDriver *drv = lib->createDriver("Modbus");
//    ModbusDriverTester *drvTester = new ModbusDriverTester();
////    string strCfg("127.0.0.1:502");
////    drv->setComConfigure(strCfg);
//    drv->setComConfigure("192.168.1.46:10502");
//    drv->start();
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//    string str2 = drv->comConfigure();
//    drv->stop();

//    ASSERT_EQ(var1, str2);
//}


/**
 * @brief ModbusDriver: isDataConfigValid
 */
//TEST(Modbus, DISABLE_test002)
//{
//    vector<string> rangesOK = {"{0, 10}", "{10, 20}", "{30,   10}"};
//    vector<string> rangesNO = {"{0, 200}", "{200, x}", "0, "};

//    std::unique_ptr<ModbusDriver> drv(new ModbusDriver());
//    ModbusDriverTester drvTester;
//    bool flag = true;
//    for (auto item: rangesOK) {
//        if (!drvTester.isDataConfigValid(item)) {
//             flag = false;
//             break;
//        }
//    }
//    for (auto item: rangesNO) {
//        if (drvTester.isDataConfigValid(item)) {
//             flag = false;
//             break;
//        }
//    }
//    ASSERT_EQ(true, flag);
//}


/**
 * @brief ModbusDriver: isDataConfigValid
 */
//TEST(Modbus, DISABLE_test003)
//{
//    vector<string> ranges = {"{0, 10}"};
//    uint16_t const_addr = 0;
//    uint16_t const_value = 20;

//    std::unique_ptr<ModbusDriver> drv(new ModbusDriver());
//    drv->setComConfigure("192.168.1.46:10502");
//    std::string conf("{0,5}");
//    drv->setDataConfigure(conf);
//    drv->setRequestDelay(100);
//    drv->start();
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    std::cout << "write: " << const_value << std::endl;
//    drv->write(const_addr, const_value);
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    IDriver::Data data = drv->read();
//    uint16_t value = data.regs.at(0);
//    std::cout << "read: " << value << std::endl;
//    drv->stop();

//    ASSERT_EQ(const_value, value);
//}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(print_time) = true;
    return RUN_ALL_TESTS();
}
