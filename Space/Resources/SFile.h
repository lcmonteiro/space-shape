/**
 * ------------------------------------------------------------------------------------------------
 * File:   SFile.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SFILE_H
#define SFILE_H
/**
 */
#include "SStream.h"
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Files Types Adapters
 * ------------------------------------------------------------------------------------------------
 */
namespace SAdapter { 
    struct SFile {
        bool Exist(int fd);
    };
    struct SFileAppend : public SFile {
        int Open(const String& path);
    };
    struct SFileTrunc : public SFile {
        int Open(const String& path);
    };
    struct SFileRead : public SFile {
        int Open(const String& path);
    };
    struct SFileNormal : public SFile {
        int Open(const String& path);
    };
}
/**
 * ------------------------------------------------------------------------------------------------
 * file template
 * ------------------------------------------------------------------------------------------------
 */
template <class T> 
class SFileT : public SStream, private T {
public:
    /**
     * ----------------------------------------------------
     * constructors
     * ----------------------------------------------------
     * main
     */
    SFileT(const String& path): SStream(T::Open(path)), 
    __path(path) {}
    /**
     * default
     */
    SFileT()          = default;
    SFileT(SFileT &&) = default;
    /**
     * ------------------------------------------------------
     * move operator
     * ------------------------------------------------------
     */
    SFileT& operator=(SFileT && file) = default;
    /**
     * ------------------------------------------------------
     * utilities
     * ------------------------------------------------------
     */
    inline void link() {
        SFileT::operator=(SFileT(path()));
    }
    inline String path() {
        return __path;
    }
    inline Boolean linked() {
        return T::Exist(ohandler());
    }
private:
    /**
     * ----------------------------------------------------
     * path
     * ----------------------------------------------------
     */
    String __path;
};
/**
 * ------------------------------------------------------------------------------------------------
 * Base File Types 
 * ------------------------------------------------------------------------------------------------
 */
typedef SFileT<SAdapter::SFileAppend> Appender;
typedef SFileT<SAdapter::SFileTrunc>  Writer;
typedef SFileT<SAdapter::SFileRead>   Reader;
typedef SFileT<SAdapter::SFileNormal> Editor;
/**
 * ------------------------------------------------------------------------------------------------
 * Extended File Types
 * ------------------------------------------------------------------------------------------------
 */
typedef class SLogger : public Appender {
public:
    /**
     * ------------------------------------------------------------------------
     * Main constructor
     * @param path - file path
     * @param size - max file size
     * @param n - number of files
     * ------------------------------------------------------------------------
     */
    SLogger(String path, Integer size = 100000000, Integer n = 10) 
    : Appender(path), _size(size), _n(n) { }
    /**
     * Empty constructor
     */
    SLogger(): Appender(), _size(0), _n(0) { }
    /**
     * ------------------------------------------------------------------------
     * move operator
     * ------------------------------------------------------------------------
     */
    SLogger& operator=(SLogger && file) {
        /**
         * move resource
         */
        Appender::operator=(std::move(file));
        /**
         * move parameters
         */
        _size = std::move(file._size);
        _n    = std::move(file._n);
        /** 
         */
        return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * config 
     * ------------------------------------------------------------------------
     */
    inline SLogger& size(Integer sz) {
        _size = sz; 
        return *this;
    }
    inline SLogger& rotate(Integer n) {
        _n = n; return *this;
    }
    /**
     * ------------------------------------------------------------------------
     * I/O  
     * ------------------------------------------------------------------------
     */
    template <class V>
    inline std::ostream& operator<<(V v) {
        if (__overload()){
            /**
             * rotate file names
             */
            __rotate();
            /**
             * create a new link
             */
            link();
        }
        return Appender::operator<<(v);
    }
protected:
    Integer _size;
    Integer _n;
private:
    using Appender::operator>>;
    /**
     * check file size
     */
    bool __overload();
    /**
     * rotate file
     */
    void __rotate();
} Logger;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SFILE_H */

