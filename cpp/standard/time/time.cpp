
#include <chrono>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <string>
#include <time.h>

using namespace std;

inline std::time_t from_iso8601(std::string descriptor)
{
    // Parse according to ISO 8601
    std::tm t;

    const char *value = descriptor.c_str();
    const char *c = value;
    int year = 0;
    int month = 0;
    int seconds = 0;
    int minutes = 0;
    int hours = 0;
    int days = 0;

    // NOTE: we have to check for the extended format first,
    // because otherwise the separting '-' will be interpreted
    // by std::sscanf as signs of a 1 digit integer .... :-(
    if (std::sscanf(value, "%4u-%2u-%2u", &year, &month, &days) == 3)
    {
        c += 10;
    }
    else if (std::sscanf(value, "%4u%2u%2u", &year, &month, &days) == 3)
    {
        c += 8;
    }
    else
    {
        throw std::runtime_error(std::string("Invalid date format: ") + value);
    }

    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = days;

    // Check if time is supplied
    if (*c == '\0')
    {
        t.tm_hour = 0;
        t.tm_sec = 0;
        t.tm_min = 0;
    }
    else if (*c == 'T')
    {
        // Time of day part
        c++;

        if (std::sscanf(c, "%2d%2d", &hours, &minutes) == 2)
        {
            c += 4;
        }
        else if (std::sscanf(c, "%2d:%2d", &hours, &minutes) == 2)
        {
            c += 5;
        }
        else
        {
            throw std::runtime_error(std::string("Invalid date format: ") + value);
        }

        if (*c == ':')
        {
            c++;
        }

        if (*c != '\0')
        {
            if (std::sscanf(c, "%2d", &seconds) == 1)
            {
                c += 2;
            }
            else
            {
                throw std::runtime_error(std::string("Invalid date format: ") + value);
            }
        }

        t.tm_hour = hours;
        t.tm_min = minutes;
        t.tm_sec = seconds;
    }
    else
    {
        throw std::runtime_error(std::string("Invalid date format: ") + value);
    }

    // Drop microsecond information
    if (*c == '.')
    {
        c++;

        while (std::isdigit(*c) && *c != '\0')
        {
            c++;
        }
    }

    // Parse time zone information
    int tz_offset = 0;

    if (*c == 'Z')
    {
        c++;
    }

    if (*c != '\0')
    {
        int tz_direction = 0;

        if (*c == '+')
        {
            tz_direction = 1;
        }
        else if (*c == '-')
        {
            tz_direction = -1;
        }
        else
        {
            throw std::runtime_error(std::string("Invalid date format: ") + value);
        }

        // Offset part
        int tz_hours = 0;
        int tz_minutes = 0;

        c++;

        if (std::sscanf(c, "%2d", &tz_hours) == 1)
        {
            c += 2;
        }
        else
        {
            throw std::runtime_error(std::string("Invalid date format: ") + value);
        }

        if (*c == ':')
        {
            c++;
        }

        if (*c != '\0')
        {
            if (std::sscanf(c, "%2d", &tz_minutes) == 1)
            {
                c += 2;
            }
            else
            {
                throw std::runtime_error(std::string("Invalid date format: ") + value);
            }
        }

        tz_offset = tz_direction * (tz_hours * 3600 + tz_minutes * 60);
    }

    // Determine DST automatically
    t.tm_isdst = -1;

    // Getting gmtoff
    const auto a = std::mktime(&t);
    tz_offset -= t.tm_gmtoff;

    return std::mktime(&t) - tz_offset;
}

std::time_t undefinedTime()
{
    return std::numeric_limits<std::time_t>::max();
}

std::time_t timeFromISO8601(const std::string &iso8601, bool *result = nullptr)
{
    try
    {
        const auto out = from_iso8601(iso8601);
        if (result)
            *result = true;
        return out;
    }
    catch (const std::exception &exc)
    {
        if (result)
            *result = false;
    }
    return undefinedTime();
}

std::time_t localtimeFromISO8601(const std::string &iso8601, bool *result  = nullptr )
{
    try
    {
        const auto out = from_iso8601(iso8601);
        const std::time_t curTime = std::time(nullptr);
        const int64_t gmtoff = std::localtime(&curTime)->tm_gmtoff;
        const auto gmtime = out - gmtoff;
        assert(gmtime >= 0);
        if (gmtime < 0)
        {
            throw std::runtime_error("time is negative");
        }
        if (result)
            *result = true;
        return gmtime;
    }
    catch (const std::exception &exc)
    {
        if (result)
            *result = false;
    }
    return undefinedTime();
}

int64_t today()
{
    time_t result = time(NULL); //std::time(nullptr);
    tm *tm = localtime(&result);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return int64_t(mktime(tm));
}

int main()
{
    assert(0);
    cout << timeFromISO8601("1970-01-01T00:00:00Z") << endl;
    cout << localtimeFromISO8601("1970-01-01T00:00:00Z") << endl;
    // std::time_t curTime = std::time(nullptr);
    // cout << a - localtime(&curTime)->tm_gmtoff << endl;
    // fun();

    // std::tm t;
    // cout << t.tm_gmtoff << endl;
    // cout << std::mktime(&t) << endl;
    // cout << t.tm_gmtoff << endl;
    // cout << today() << endl;

    return 0;
}