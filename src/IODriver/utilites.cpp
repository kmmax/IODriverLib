#include "utilites.h"


Utilites::Time::Time()
{

}


/*!
 * \brief Converts std::chrono::system_clock::time_point to string
 *
 * Conversion depends from format flag: For example:
 * - TimeFormat::Time:          "12:23:55.963"
 * - TimeFormat::Date:          "07.07.2019"
 * - TimeFormat::DateTime:      "07.07.2019 12:23:55.963"
 * \param[in] tp - input timepoint (time_point)
 * \return string in format "hh:mm:ss.ms"
 */
string Utilites::Time::timePointToString(Utilites::Time::TimePoint tp, TimeFormat format /*TimeFormat::TimeOnly*/)
{
    using namespace std;
    using namespace std::chrono;

    time_t rawTime = Clock::to_time_t(tp);
    struct tm *tmTime;
    tmTime = localtime(&rawTime);

    int year = tmTime->tm_year + 1900;
    int month = tmTime->tm_mon;
    int day = tmTime->tm_mday;
    int hour = tmTime->tm_hour;
    int min = tmTime->tm_min;
    int sec = tmTime->tm_sec;
    long long ms = duration_cast<milliseconds>(tp.time_since_epoch()).count() % 1000;

    stringstream ss;
    ss.fill('0');
    switch (format) {
    case TimeFormat::Date:
        ss << setw(2) << day << "." << setw(2) << month << "." << setw(4) << year;
        break;
    case TimeFormat::Time:
        ss << setw(2) << hour << ":" << setw(2) << min << ":" << setw(2) << sec << "." << setw(3) << ms;
        break;
    case TimeFormat::DateTime:
        ss << setw(2) << day << "." << setw(2) << month << "." << setw(4) << year << " ";
        ss << setw(2) << hour << ":" << setw(2) << min << ":" << setw(2) << sec << "." << setw(3) << ms;
        break;
    default:
        ss << "Unknown format";
    }
    string str = ss.str();

    return str;
}



/*!
 * \brief Extract substring in tokens
 * \param[in] src - source string;
 * \param[in] start - start token;
 * \param[in] end - end token
 * \return list of substring
 * \code
 *   string src("{0,10} {5,15}");
 *   string m1("{");
 *   string m2("}");
 *   auto v = substrByToken(src, m1, m2);
 *   // v[0] = "0,10"
 *   // v[1] = "5,15"
 * \endcode
 *
 */
std::vector<string> Utilites::substrByToken(const string &src, const string &start, const string &end)
{
    vector<string> res;

    auto posStart = src.find(start);
    auto posStop = src.find(end);
    if (posStart < posStop) {
        while (string::npos != posStart && string::npos != posStop) {
            string tmp = src.substr(posStart+start.size(), posStop-posStart-start.size());
            res.push_back(tmp);
            posStart = src.find(start, posStop);
            posStop = src.find(end, posStart);
        }
    }

    return res;
}
