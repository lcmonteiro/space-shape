/**
 * ------------------------------------------------------------------------------------------------
 * File:   JsonConverter.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * -------------------------------------------------------------------------------------------------
 */
#ifndef JSONCONVERT_H
#define JSONCONVERT_H
/**
 * std
 */
#include <iostream>
#include <regex>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Json Converter
 * ------------------------------------------------------------------------------------------------
 */
namespace Convert {
    /**
     * ------------------------------------------------------------------------
     * To Json
     * ------------------------------------------------------------------------
     */  
    std::ostream& ToJson(std::ostream&& os, Var  v);
    std::ostream& ToJson(std::ostream& os,  Var  v);
    std::ostream& ToJson(std::ostream& os,  List l);
    std::ostream& ToJson(std::ostream& os,  Map  m);
    /**
     * ------------------------------------------------------------------------
     * To Pretty Json
     * ------------------------------------------------------------------------
     */
    std::ostream& ToPrettyJson(std::ostream&& os, Var  v);
    std::ostream& ToPrettyJson(std::ostream& os,  Var  v);
    std::ostream& ToPrettyJson(std::ostream& os,  List l);
    std::ostream& ToPrettyJson(std::ostream& os,  Map  m);
    /**
     * ------------------------------------------------------------------------
     * From Json
     * ------------------------------------------------------------------------
     */
    Var FromJson(std::istream&& is);
    Var FromJson(std::istream&  is);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Var Serializer
 * ------------------------------------------------------------------------------------------------
 */
inline std::ostream& operator<<(std::ostream& os, const Var& data) {
    return Convert::ToPrettyJson(os, data); 
}
inline std::ostream& operator<<(std::ostream& os, const Map&  data) {
    return Convert::ToPrettyJson(os, data);
}
inline std::ostream& operator<<(std::ostream& os, const List& data) {
    return Convert::ToPrettyJson(os, data);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif  /* JSONCONVERT_H */

