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
#include <functional>
/**
 * space
 */
#include "STime.h"
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Log Macros
 * ------------------------------------------------------------------------------------------------
 * Debug
 * --------------------------------------------------------
 */
#define DEBUG(_tag_, _msg_) do {                     \
    std::lock_guard<std::mutex> lck(SLog::Locker()); \
    SLog::Debug()                                    \
        << "[ " << STime::ToDateTime(STime::Time()) << " ] "\
        << "[  DEBUG  ] "                            \
        << "[ " << _tag_ << " (" << __func__ <<") ] "\
        << "[ " << _msg_ <<" ]"                      \
        << std::endl;                                \
} while(0)
/**
 * --------------------------------------------------------
 * Information
 * --------------------------------------------------------
 */
#define INFO(_tag_, _msg_) do {                      \
    std::lock_guard<std::mutex> lck(SLog::Locker()); \
    SLog::Info()                                     \
        << "\e[32m"                                  \
        << "[ " << STime::ToDateTime(STime::Time()) << " ] "\
        << "[  INFO   ] "                            \
        << "[ " << _tag_ << " ] "                    \
        << "[ " << _msg_ << " ]"                     \
        << "\e[0m"                                   \
        << std::endl;                                \
} while(0)
/**
 * --------------------------------------------------------
 * Warning
 * --------------------------------------------------------
 */
#define WARNING(_tag_, _msg_) do {                   \
    std::lock_guard<std::mutex> lck(SLog::Locker()); \
    SLog::Warning()                                  \
        << "\e[33m"                                  \
        << "[ " << STime::ToDateTime(STime::Time()) << " ] "\
        << "[ WARNING ] "                            \
        << "[ " << _tag_ << " ]"                     \
        <<" [ " << _msg_ << " ]"                     \
        << "\e[0m"                                   \
        << std::endl;                                \
} while(0)
/**
 * --------------------------------------------------------
 * Error
 * --------------------------------------------------------
 */
#define ERROR(_tag_, _msg_) do {                     \
    std::lock_guard<std::mutex> lck(SLog::Locker()); \
    SLog::Error()                                    \
        << "\e[31m"                                  \
        << "[ " << STime::ToDateTime(STime::Time()) << " ] "\
        << "[  ERROR  ] "                            \
        << "[ " << _tag_ <<" ] "                     \
        << "[ " << _msg_ <<" ]"                      \
        << "\e[0m"                                   \
        << std::endl;                                \
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
    static inline void SetLevel(Level l) {
        __level = l;
    }
    /**
     * set log streams
     */
    static inline void SetDebug  (std::ostream& os) {
        __debug   = os;
    }
    static inline void SetInfo   (std::ostream& os) {
        __info    = os;
    }
    static inline void SetWarning(std::ostream& os) {
        __warning = os;
    }
    static inline void SetError  (std::ostream& os) {
        __error   = os;
    }
    static inline void SetNull   (std::ostream& os) {
        __none    = os;
    }
    /**
     * ------------------------------------------------------------------------
     *  Resources
     * ------------------------------------------------------------------------
     * streams
     */
    static inline std::ostream& Debug() {
        return __level > Level::DEBUG   ? __none : __debug;
    }
    static inline std::ostream& Info() {
        return __level > Level::INFO    ? __none : __info;
    }
    static inline std::ostream& Warning() {
        return __level > Level::WARNING ? __none : __warning;
    }
    static inline std::ostream& Error()   {
        return __level > Level::ERROR   ? __none : __error;
    }
    /**
    * atomicity
    */
    static inline std::mutex& Locker() {
        return __mutex;
    }
private:
    /**
     * ------------------------------------------------------------------------
     * Variables
     * ------------------------------------------------------------------------
     * log level
     */
    static Level __level;
    /**
     * output streams 
     */
    static std::reference_wrapper<std::ostream> __debug;
    static std::reference_wrapper<std::ostream> __info;
    static std::reference_wrapper<std::ostream> __warning;
    static std::reference_wrapper<std::ostream> __error;
    static std::reference_wrapper<std::ostream> __none;
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
