/**
 * ------------------------------------------------------------------------------------------------
 * File:   Normalize.cpp
 * Author: Luis Monteiro
 *
 * Created on Mai 25, 2019, 14:11 PM
 * ------------------------------------------------------------------------------------------------
 **
 * std
 */
#include <regex>
/**
 * space 
 */
#include "SLog.h"
#include "SLogic.h"
#include "SLearn.h"
#include "SFileSystem.h"
/**
 * ----------------------------------------------------------------------------
 * Helpers
 * ----------------------------------------------------------------------------
 */
/**
 * ----------------------------------------------------------------------------
 * Normalize
 * ----------------------------------------------------------------------------
 */
Var Command::Explorer(const String& path, String pattern) { 
    Var var = Obj(nullptr);
    /**
     * read from file system
     */
    var = FileSystem::Find(path, [&pattern](auto path, auto type) {
        return (FileSystem::DIR == type) 
            || std::regex_match(path.back(), std::regex(pattern));
    });
    /**
     * filter empty folders
     */ 
    var = Logic::ForEach(var, [](auto, auto var) {
        return Obj(3);
        //return (Var::ToInteger(var) == FileSystem::DIR)? Obj(nullptr) : var;  
    });
    /**
     * return 
     */
    return var;
}  
/**
 * ----------------------------------------------------------------------------
 * Normalize
 * ----------------------------------------------------------------------------
 */
Var Command::Learn(const List& files) {
    return {};
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */