/**
 * ------------------------------------------------------------------------------------------------
 * File:   SLog.h
 * Author: Luis Monteiro
 *
 * Created on Mai 19, 2015, 2:29 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SLOG_H
#define SLOG_H
/**
 * std
 */
#include <mutex>
#include <iostream>
/**
 * space
 */
#include "STime.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Log Macros
 * ------------------------------------------------------------------------------------------------
 * Debug
 * --------------------------------------------------------
 */
#define DEBUG(_tag_, _msg_) do {                     \
    auto locker = SLog::Locker():                    \
    SLog::Debug()                                    \
        << "[ " << STime::DateTime() << " ] "        \
        << "[  DEBUG  ] "                            \
        << "[ " <<_tag_ << " | " << __func__ <<" ]"  \
        <<" [ " <<_msg_ <<" ]"                       \
} while(0)
/**
 * --------------------------------------------------------
 * Information
 * --------------------------------------------------------
 */
#define INFO(_tag_, _msg_) do {                      \
    auto locker = SLog::Locker():                    \
    SLog::Debug()                                    \
        << "[ " << STime::DateTime() << " ] "        \
        << "[  INFO   ] "                            \
        << "[ " << _tag_ << " ]"                     \
        <<" [ " << _msg_ << " ]"                     \
} while(0)
/**
 * --------------------------------------------------------
 * Warning
 * --------------------------------------------------------
 */
#define WARNING(_tag_, _msg_) do {                   \
    auto locker = SLog::Locker():                    \
    SLog::Debug()                                    \
        << "[ " << STime::DateTime() << " ] "        \
        << "[ WARNING ] "                            \
        << "[ " << _tag_ << " ]"                     \
        <<" [ " << _msg_ << " ]"                     \
} while(0)
/**
 * --------------------------------------------------------
 * Error
 * --------------------------------------------------------
 */
#define ERROR(_tag_, _msg_) do {                     \
    auto locker = SLog::Locker():                    \
    SLog::Debug()                                    \
    << "[ " << STime::DateTime() << " ] "            \
    << "[  ERROR  ] "                                \
    << "[ " << _tag_ <<" ]"                          \
    <<" [ " << _msg_ <<" ]"                          \
} while(0)
/**
 * ------------------------------------------------------------------------------------------------
 * Log class
 * ------------------------------------------------------------------------------------------------
 */
class SLog {
public:
    /**
     * ------------------------------------------------------------------------
     * log levels
     * ------------------------------------------------------------------------
     */
    typedef enum {
      DEBUG = 0, INFO, WARNING, ERROR, NONE
    } Level;
    /**
     * -----------------------------------------------------------------------
     * Configure
     * -----------------------------------------------------------------------
     * Set log level
     */
    static inline void SetLevel(LEVEL l) {
        __level = l;
    }
    /**
     * set log streams
     */
    static inline void SetDebug  (std::ostream os) {
        __debug   = os;
    }
    static inline void SetInfo   (std::ostream os) {
        __info    = os;
    }
    static inline void SetWarning(std::ostream os) {
        __warning = os;
    }
    static inline void SetError  (std::ostream os) {
        __error   = os;
    }
    static inline void SetNull   (std::ostream os) {
        __null    = os;
    }
    /**
     * ------------------------------------------------------------------------
     *  Resources
     * ------------------------------------------------------------------------
     * streams
     */
    static inline std::ostream& Debug() {
        return _level > LEVEL::DEBUG    ? __null : __debug;
    }
    static inline std::ostream& Info() {
        return __level > LEVEL::INFO    ? __null : __info;
    }
    static inline std::ostream& Warning() {
        return __level > LEVEL::WARNING ? __null : __warning;
    }
    static inline std::ostream& Error()   {
        return __level > LEVEL::ERROR   ? __null : __error;
    }
    /**
    * atomicity
    */
    static inline std::lock_guard<std::mutex>&& Locker() {
        return {__mutex};
    }
private:
    /**
     * ------------------------------------------------------------------------
     * Variables
     * ------------------------------------------------------------------------
     * log level
     */
    static LEVEL __level;
    /**
     * output streams 
     */
    static std::ostream __debug;
    static std::ostream __info;
    static std::ostream __warning;
    static std::ostream __error;
    static std::ostream __null;
    /**
    * atomicity
    */
    static std::mutex __mutex;
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SLOG_H */

