/**
 * ------------------------------------------------------------------------------------------------
 * File:   XmlConverter.h
 * Author: Luis Monteiro
 *
 * Created on September 14, 2015, 4:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H
/**
 * std
 */
#include <sstream>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * XmlConverter
 * ------------------------------------------------------------------------------------------------
 */
namespace Convert {
    /**
     * ------------------------------------------------------------------------
     * format types
     * ------------------------------------------------------------------------
     */
    typedef enum {
        RAW, FILE, RAW_NODE 
    } FORMAT;
    /**
     * ------------------------------------------------------------------------
     *  To XML
     * ------------------------------------------------------------------------
     */
    std::ostream& ToXML(
        std::ostream&  os, Var data, FORMAT format = FORMAT::FILE);
    std::ostream& ToXML(
        std::ostream&& os, Var data, FORMAT format = FORMAT::FILE); 
    /**
     * ------------------------------------------------------------------------
     *  From XML
     * ------------------------------------------------------------------------
     */
    Var FromXML(std::istream&  is);
    Var FromXML(std::istream&& is);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* XMLCONVERTER_H */

