/**
 * ------------------------------------------------------------------------------------------------
 * File:   SFind.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef FIND_H
#define FIND_H
/**
 * space
 */
#include "SBasic.h"
#include "SLogic.h"
#include "SConvert.h"
#include "SFileSystem.h"
/**
 * namespaces
 */
using namespace Tools;
/**
 * ----------------------------------------------------------------------------
 * Find files
 * ----------------------------------------------------------------------------
 */
static inline List Find(String path, String pattern) {
    /**
     * read from file system
     */
    Var aux = FileSystem::Find(path, [&pattern](auto path, auto type) {
        return (FileSystem::DIR == type) 
            || std::regex_match(path.back(), std::regex(pattern));
    });
    /**
     * filter empty folders
     */ 
    aux = Var::Trim(Logic::ForEach(aux, [](auto, auto var) {
        if(Var::IsInteger(var)) {
            return (Var::Integer(var) == FileSystem::DIR)? Obj(nullptr) : var;  
        }
        return var;
    }));
    /**
     * return a list of path files
     */
    return Logic::ForEach(
        Basic::GetKeys(Convert::ToSimpleMap(aux, "/")), [&path](auto, auto var){
            return Obj(String::Build(path, "/", Var::String(var)));
        }
    );
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* FIND_H */