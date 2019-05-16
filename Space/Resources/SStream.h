/**
 * ------------------------------------------------------------------------------------------------
 * File:   SStream.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SSTREAM_H
#define SSTREAM_H
/**
 * std
 */
#include <iostream>
#include <fstream>
/**
 * space
 */
#include "SConvertJSON.h"
#include "SConvertBIN.h"
#include "SResource.h"
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * List of exceptions
 * ------------------------------------------------------------------------------------------------
 */
typedef class SStreamException : public std::runtime_error {
public:
    using runtime_error::runtime_error;
} StreamException;
typedef class SIStreamException : public SStreamException {
public:
    using SStreamException::SStreamException;
} IStreamException;
typedef class SOStreamException : public SStreamException {
public:
    using SStreamException::SStreamException;
} OStreamException;
/**
 * ------------------------------------------------------------------------------------------------
 * Stream
 * ------------------------------------------------------------------------------------------------
 */
typedef class SStream : public SResource {
public:
    /**
     * ------------------------------------------------------------------------
     * destructor
     * ------------------------------------------------------------------------
     */
    virtual ~SStream();
    /**
     * ------------------------------------------------------------------------
     * swap
     * ------------------------------------------------------------------------
     */
    inline SStream& operator=(SStream && res) {
        /**
         * swap streams
         */
        res.__is.rdbuf(__is.rdbuf(res.__is.rdbuf()));
        res.__os.rdbuf(__os.rdbuf(res.__os.rdbuf()));
        /**
         * swap resources
         */
        SResource::operator=(std::move(res));
        /**
         */
        return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * overrides
     * ------------------------------------------------------------------------
     */
    Boolean igood() override {
        if (!SResource::igood() || __is.sync() < 0) {
            return false;
        }
        return true;
    }
    Boolean ogood() {
        if (!SResource::ogood() || !__os.flush().good()) {
            return false;
        }
        return true;
    }
    Boolean good() {
        if (!SResource::good()) {
            return false;
        }
        if (SResource::igood() && __is.sync() < 0) {
            return false;
        }
        if (SResource::ogood() && !__os.flush().good()) {
            return false;
        }
        return true;
    }
    /**
     * ------------------------------------------------------------------------
     * sync streams
     * ------------------------------------------------------------------------
     */
    SStream& sync() {
        if (SResource::igood() && __is.sync() < 0) {
            throw IStreamException("bad resource");
        }
        if (SResource::ogood() && !__os.flush().good() < 0) {
            throw OStreamException("bad resource");
        }
        return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * load data from OS to stream
     * ------------------------------------------------------------------------
     */
    std::istream& iload() {
        if (SResource::igood() && __is.peek() < 0) {
            throw IStreamException("bad resource");
        }
        return __is;
    }
    /**
     * ------------------------------------------------------------------------
     * streams operators
     * ------------------------------------------------------------------------
     */
    inline operator std::ostream& () {
        return __os;
    }
    inline operator std::istream& () {
        return __is;
    }
    /**
     * ------------------------------------------------------------------------
     * streams
     * ------------------------------------------------------------------------
     */
    inline std::istream& in() {
        return __is;
    }
    inline std::ostream& out() {
        return __os;
    }
    /**
     * ------------------------------------------------------------------------
     * enable exceptions 
     * ------------------------------------------------------------------------
     */
    inline SStream& execptions(Boolean enable) {
        __is.exceptions(enable ? std::ios::badbit : std::ios::goodbit);
        __os.exceptions(enable ? std::ios::badbit : std::ios::goodbit);
        return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * set exceptions
     * ------------------------------------------------------------------------
     */
    template <class S>
    static S& SetException(S& s, std::ios::iostate except = std::ifstream::failbit | std::ifstream::badbit) {
        s.exceptions(except);
        return s;
    }
    template <class S>
    static S& SetException(S&& s, std::ios::iostate except = std::ifstream::failbit | std::ifstream::badbit) {
        s.exceptions(except);
        return s;
    }
    /**
     * ------------------------------------------------------------------------
     * clear stream errors 
     * ------------------------------------------------------------------------
     */
    inline SStream& clear() {
        __is.clear();
        __os.clear();
        return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * IO functions
     * ------------------------------------------------------------------------
     */
    inline Var read() {
        return Convert::FromBin(__is);
    }
    inline std::istream& read(Var& v) {
        v = Convert::FromBin(__is);
        return __is;
    }
    inline std::ostream& write(Var v) {
        return Convert::ToBin(__os, v); 
    }
    inline std::ostream& operator<<(Var v) {
        return Convert::ToJson(__os, v);
    }
    inline std::istream& operator>>(Var& v) {
        v = Convert::FromJson(__is);
        return __is;
    }
    template <class V>
    inline std::ostream& operator<<(V v) {
        return __os << v;
    }
    template <class V>
    inline std::istream& operator>>(V& v) {
        return __is >> v;
    }
    /**
     * ------------------------------------------------------------------------
     * utilities
     * ------------------------------------------------------------------------
     */
    static std::istream& Input(SResource& res) {
        return static_cast<SStream&>(res).in();
    }
    static std::ostream& Output(SResource& res) {
        return static_cast<SStream&>(res).out();
    }
protected:
    /**
     * ------------------------------------------------------------------------
     * friend class
     * ------------------------------------------------------------------------
     */
    friend class SFileSystem;
    /**
     * ------------------------------------------------------------------------
     * constructors
     * ------------------------------------------------------------------------
     */
    SStream(int io);
    SStream(int i, int o);
    // empty
    SStream() : SResource(), __is(nullptr), __os(nullptr) {}
    // move
    SStream(SStream&& res): SStream() { *this = std::move(res); }
private:
    /**
     * ------------------------------------------------------------------------
     * streams
     * ------------------------------------------------------------------------
     */
    std::istream __is;
    std::ostream __os;
} Stream;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SSTREAM_H */

