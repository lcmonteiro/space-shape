/**
 * ------------------------------------------------------------------------------------------------
 * File:   SFile.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SFILESTREAM_H
#define SFILESTREAM_H
/**
 * std
 */
#include <fstream>
/**
 * ------------------------------------------------------------------------------------------------
 * Files Types 
 * ------------------------------------------------------------------------------------------------
 */
namespace File {
    /**
     * ------------------------------------------------------------------------
     * Appender
     * ------------------------------------------------------------------------
     */
    class Appender : public std::ofstream {
    public:
        Appender(const String& path) 
        : std::ofstream(path , std::ofstream::out | std::ofstream::app) {}
        /**
         * move constructor
         */
        Appender(Appender&& r) : std::ofstream(std::forward<Appender&&>(r)) {}
        /**
         * move operator
         */
        Appender& operator=(Appender&&) = default;
    };
    /**
     * ------------------------------------------------------------------------
     * Writer
     * ------------------------------------------------------------------------
     */
    class Writer : public std::ofstream {
    public:
        Writer(const String& path) 
        : std::ofstream(path , std::ofstream::out | std::ofstream::trunc) {}
        /**
         * move constructor
         */
        Writer(Writer&& r) : std::ofstream(std::forward<Writer&&>(r)) {}
        /**
         * move operator
         */
        Writer& operator=(Writer&&) = default;
    };
    /**
     * ------------------------------------------------------------------------
     * Reader
     * ------------------------------------------------------------------------
     */
    class Reader : public std::ifstream {
    public:
        Reader(const String& path) 
        : std::ifstream(path , std::ifstream::in) {}
        /**
         * move constructor
         */
        Reader(Reader&& r) : std::ifstream(std::forward<Reader&&>(r)) {}
        /**
         * move operator
         */
        Reader& operator=(Reader&&) = default;
    };
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SFILESTREAM_H */

