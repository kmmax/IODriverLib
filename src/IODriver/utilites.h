#ifndef UTILITES_H
#define UTILITES_H

#include <iostream>
#include <chrono>
#include <string.h>
#include <sstream>
#include <iomanip>  // for io manipulaters like setw
#include <vector>

using namespace std::chrono;
using namespace std;

namespace  Utilites {

    /*!
     * \brief Extended class for working with time
     */
    class Time
    {
    public:
        using Clock = std::chrono::system_clock;
        using TimePoint = Clock::time_point;

        enum class TimeFormat {
            DateTime,
            Date,
            Time
        };

        Time();
        // Data and time converting
        static string timePointToString(TimePoint tp, TimeFormat format = TimeFormat::DateTime);
    };

    // For suppress <Unussed variable> warning
    template<class T>
    void ignore(const T&) {}

    std::vector<std::string> substrByToken(const string &src, const string &start, const string &end);
}


#endif // UTILITES_H
