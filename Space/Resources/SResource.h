/**
 * ------------------------------------------------------------------------------------------------
 * File:   SResource.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SRESOURCE_H
#define SRESOURCE_H
/**
 * std
 */
#include <cmath>
#include <list>
#include <map>
#include <iostream>
/**
 * space
 */
#include "STypes.h"
/**
 * ------------------------------------------------------------------------------------------------
 * List of exceptions
 * ------------------------------------------------------------------------------------------------
 */
typedef class SResourceException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
} ResourceException;
typedef class SIResourceException : public SResourceException {
public:
    using SResourceException::SResourceException;
} IResourceException;
typedef class SOResourceException : public SResourceException {
public:
    using SResourceException::SResourceException;
} OResourceException;
/**
 * ------------------------------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------------------------------
 */
#define RESOURCES_SELECTED(resource, timeout)        \
    switch ((SResource::WaitFor(resource, timeout)))
/**
 * ------------------------------------------------------------------------------------------------
 * Resource
 * ------------------------------------------------------------------------------------------------
 */
class SResource {
public:
    /**
     * --------------------------------------------------------------------
     * helpers
     * --------------------------------------------------------------------
     */
    using RMap     = std::map<Integer, SResource*>; 
    using RList    = std::vector<SResource*>; 
    using Handler = int;
    /**
     * --------------------------------------------------------------------
     * destructor
     * --------------------------------------------------------------------
     */
    virtual ~SResource();
    /**
     * --------------------------------------------------------------------
     * swap
     * --------------------------------------------------------------------
     */
    inline SResource& operator=(SResource && res) {
        std::swap(__ih, res.__ih);
        std::swap(__oh, res.__oh);
        return *this;
    }
    /**
     * --------------------------------------------------------------------
     * interfaces
     * --------------------------------------------------------------------
     */
    virtual Boolean igood() {
        return !(__ih < 0);
    }
    virtual Boolean ogood() {
        return !(__oh < 0 );
    }
    virtual Boolean good() {
        return !(__ih < 0 && __oh < 0);
    }
    /**
     * --------------------------------------------------------------------
     * handlers
     * --------------------------------------------------------------------
     */
    inline Handler ihandler() {
        return __ih;
    }
    inline Handler ohandler() {
        return __oh;
    }
    /**
     * --------------------------------------------------------------------
     * waiting functions
     * --------------------------------------------------------------------
     */
    static Integer WaitFor(SResource* pResource, Float timeout);
    
    static std::vector<Integer> WaitFor(RList list, Float timeout);

    static std::vector<Integer> WaitFor(RMap map, Float timeout);
protected:
    /**
     * --------------------------------------------------------------------
     * friend class
     * --------------------------------------------------------------------
     */
    friend class SStream;
    /**
     * ------------------------------------------------------------------------
     * constructors
     * ------------------------------------------------------------------------
     */
    SResource(Handler io);
    SResource(Handler i, Handler o);
    // empty
    SResource() : __ih(-1), __oh(-1) { }
    // move
    SResource(SResource&& res) : SResource() {
        *this = std::move(res);
    }
    /**
     * ------------------------------------------------------------------------
     *  IO Functions
     * ------------------------------------------------------------------------
     */
    template<class T>
    inline Boolean write(T value) {
        return (__write(&value.ref(), value.size()) == value.size());
    }
    template<class T>
    inline Boolean read(T& value) {
        return (__read(&value.ref(), value.size()) == value.size());
    }
    /**
     * ------------------------------------------------------------------------
     * detach
     * ------------------------------------------------------------------------
     */
    inline void clear() {
        __ih = __oh = -1;
    }
private:
    /**
     * ------------------------------------------------------------------------
     * native handlers
     * ------------------------------------------------------------------------
     */
    Handler __ih;
    Handler __oh;
    /**
     * ------------------------------------------------------------------------
     * IO functions
     * ------------------------------------------------------------------------
     */
    size_t __write(void* p, size_t s);
    size_t __read(void* p,  size_t s);
};


#endif /* SRESOURCE_H */

