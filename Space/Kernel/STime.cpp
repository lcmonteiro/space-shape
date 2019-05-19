/**
 * ------------------------------------------------------------------------------------------------
 * File:   STime.cpp
 * Author: Luis Monteiro
 * 
 * Created on Mai 19, 2019, 22:29 PM
 * ------------------------------------------------------------------------------------------------
 **
 * std
 */
#include <cstring>
#include <cerrno>
#include <ctime>
#include <chrono>
#include <stdexcept>
/**
 * space
 */
#include "STime.h"
/**
 * namespaces
 */
using namespace std;
/**
 * ----------------------------------------------------------------------------
 * Get Time
 * ----------------------------------------------------------------------------
 */
const Float STime::Time() {
    struct timespec t_aux;
    if (clock_gettime(CLOCK_REALTIME, &t_aux) < 0) {
        throw runtime_error(string("clock get time: ") + strerror(errno));
    }
    return ((double) t_aux.tv_sec) + ((double) t_aux.tv_nsec / 1000000000);
}
/**
 * ----------------------------------------------------------------------------
 * Get System Time
 * ----------------------------------------------------------------------------
 */
const Float STime::SysTime() {
    struct timespec t_aux;
    if (clock_gettime(CLOCK_MONOTONIC, &t_aux) < 0) {
        throw runtime_error(string("clock get time: ") + strerror(errno));
    }
    return ((double) t_aux.tv_sec) + ((double) t_aux.tv_nsec / 1000000000);
}
/**
 * ---------------------------------------------------------------------------
 * Convert to Date Time
 * ---------------------------------------------------------------------------
 */
const String STime::ToDateTime(
    const Float& time, const String& pattern, Boolean* isdst
    ) {
    char str[64];
    struct tm tm;
    time_t t = (time_t) time;
    memset(str, 0, sizeof (str));
    memset(&tm, 0, sizeof (struct tm));
    /**
     * convert
     */
    strftime(str, sizeof (str), pattern.c_str(), localtime_r(&t, &tm));
    if (isdst) {
        *isdst = tm.tm_isdst==1;
    }
    return str;
}
/**
 * ----------------------------------------------------------------------------
 * Convert from Date Time
 * ----------------------------------------------------------------------------
 */
const Float STime::FromDateTime(
    const String& time, const String& pattern, Boolean isdst
    ) {
    struct tm tm;
    memset(&tm, 0, sizeof (struct tm));
    /**
     * convert
     */
    strptime(time.data(), pattern.c_str(), &tm);
    if (isdst) {
        tm.tm_isdst = 1;
    }
    return timelocal(&tm);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */