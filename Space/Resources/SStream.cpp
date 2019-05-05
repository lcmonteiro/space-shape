/**
 * ------------------------------------------------------------------------------------------------
 * File:   SResource.cpp
 * Author: Luis Monteiro
 *
 * Created on June 3, 2015, 10:12 AM
 * ------------------------------------------------------------------------------------------------
 */
#include <ext/stdio_filebuf.h>
#include <unistd.h>
/**
 * space
 */
#include "SStream.h"
#include "SResource.h"
/**
 * helpers
 */
using resource_buffer = __gnu_cxx::stdio_filebuf<char>;
/**
 * ----------------------------------------------------------------------------
 * constructors
 * ----------------------------------------------------------------------------
 */
SStream::SStream(int io) : SResource(io, dup(io)),
__is(new resource_buffer(ihandler(), std::ios::in  | std::ios::binary)),
__os(new resource_buffer(ohandler(), std::ios::out | std::ios::binary)) {
}
SStream::SStream(int i, int o) : SResource(i, o),
__is(new resource_buffer(i, std::ios::in  | std::ios::binary)),
__os(new resource_buffer(o, std::ios::out | std::ios::binary)){
}
/**
 * ----------------------------------------------------------------------------
 * destructor
 * ----------------------------------------------------------------------------
 */
SStream::~SStream() {
        /**
         * disable exceptions
         */
        __is.exceptions(std::ios::goodbit);
        __os.exceptions(std::ios::goodbit);
        /**
         * close
         */
        auto p_ibuf = (resource_buffer*)__is.rdbuf(nullptr);
        if(p_ibuf){
                p_ibuf->close();
                delete p_ibuf;
        }
        auto p_obuf = (resource_buffer*)__os.rdbuf(nullptr);
        if(p_obuf){
                p_obuf->close();
                delete p_obuf;
        }
        /**
         * clear native handles
         */
        SResource::clear();
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */