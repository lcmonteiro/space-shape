/**
 * ------------------------------------------------------------------------------------------------
 * File:   JsonConverter.h
 * Author: Luis Monteiro
 *
 * Created on May 14, 2015, 12:11 PM
 * -------------------------------------------------------------------------------------------------
 */
#ifndef JSONCONVERT_H
#define JSONCONVERT_H
/**
 * std
 */
#include <iostream>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
# define __PRECISION__  12
/**
 * tags
 */
#define __LIST_LD__     '['
#define __LIST_RD__     ']'
#define __MAP__LD__     '{'
#define __MAP__RD__     '}'
#define __STR___D__     '\"'
#define __ESC___D__     '\\'
#define __KEY___D__     ':'
#define __ELM___D__     ','
#define __NUM___D__     '0'
#define __1_____D__     '1'
#define __2_____D__     '2'
#define __3_____D__     '3'
#define __4_____D__     '4'
#define __5_____D__     '5'
#define __6_____D__     '6'
#define __7_____D__     '7'
#define __8_____D__     '8'
#define __9_____D__     '9'
#define __NULL__D__     'n'
/**
 */
#define __NULL__S__     "null"
#define __TAB___S__     "    "
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
     * interfaces
     */  
    static std::ostream& ToJson(std::ostream&& os, Var  v);
    static std::ostream& ToJson(std::ostream& os,  Var  v);
    static std::ostream& ToJson(std::ostream& os,  List l);
    static std::ostream& ToJson(std::ostream& os,  Map  m);
    /**
     * ------------------------------------------------------------------------
     * To Pretty Json
     * ------------------------------------------------------------------------
     * interfaces
     */
    static std::ostream& ToPrettyJson(std::ostream&& os, Var  v);
    static std::ostream& ToPrettyJson(std::ostream& os,  Var  v);
    static std::ostream& ToPrettyJson(std::ostream& os,  List l);
    static std::ostream& ToPrettyJson(std::ostream& os,  Map  m);
    /**
     * ------------------------------------------------------------------------
     * From Json
     * ------------------------------------------------------------------------
     * interfaces
     */
    static Var FromJson(std::istream&& is);
    static Var FromJson(std::istream&  is);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif  /* JSONCONVERT_H */

