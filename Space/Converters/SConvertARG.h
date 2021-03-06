/**
 * ------------------------------------------------------------------------------------------------
 * File:   ConverterARG.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef ARGCONVERTER_H
#define ARGCONVERTER_H
/**
 * std
 */
#include <map>
#include <vector>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Args Converter
 * ------------------------------------------------------------------------------------------------
 */
namespace Convert {
    /**
     * ------------------------------------------------------------------------
     *  From ARGs
     * ------------------------------------------------------------------------
     */
    Var FromARG(
        std::vector<std::string> args, 
        std::map<std::string, std::string> map={}
    );
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* ARGCONVERTER_H */

