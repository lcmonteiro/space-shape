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
#include <sstream>
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
    const Float& time, const String& pattern, Boolean* isdst) {
    time_t t = (time_t) time;
    /**
     * get time
     */
    auto* tm = std::localtime(&t);
    /**
     * process dls
     */
    if (isdst) {
        *isdst = tm->tm_isdst==1;
    }
    /**
     * convert time
     */
    std::ostringstream ss;
    ss << std::put_time(tm, pattern.data());
    return ss.str();
}
/**
 * ----------------------------------------------------------------------------
 * Convert from Date Time
 * ----------------------------------------------------------------------------
 */
const Float STime::FromDateTime(
    const String& time, const String& pattern, Boolean isdst) {
    /**
     * convert to tm structor
     */
    std::tm t = {};
    std::istringstream ss(time);
    ss >> std::get_time(&t, pattern.data());
    /**
     * process dls
     */ 
    if (isdst) {
        t.tm_isdst = 1;
    }
    /**
     * convert to seconds
     */
    return std::mktime(&t);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */