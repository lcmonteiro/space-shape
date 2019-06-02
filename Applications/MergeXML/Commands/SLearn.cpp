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
#include "SUtils.h"
#include "SConvert.h"
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
    var = Var::Trim(Logic::ForEach(var, [](auto, auto var) {
        if(Var::IsInteger(var)) {
            return (Var::Integer(var) == FileSystem::DIR)? Obj(nullptr) : var;  
        }
        return var;
    }));
    /**
     * return a list of path files
     */
    return Obj(Logic::ForEach(
        Utils::GetKeys(Convert::ToSimpleMap(var, "/")), [&path](auto, auto var){
            return Obj(String::Build(path, "/", Var::String(var)));
        }
    ));
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