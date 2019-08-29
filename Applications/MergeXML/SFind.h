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
#include "SPattern.h"
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

    switch(FileSystem::GetPathType(path)) {
        /**
         * --------------------------------------------------------------------
         * File Found
         * --------------------------------------------------------------------
         */
        case FileSystem::Resource::FILE: {
            return List{Obj(path)};
        }
        /**
         * --------------------------------------------------------------------
         * Directory Search
         * --------------------------------------------------------------------
         */
        case FileSystem::Resource::DIR: {
            /**
             * read from file system
             */
            Var aux = FileSystem::Find(path, [&pattern](auto p, auto t) {
                return (FileSystem::DIR == t) || Tools::Pattern::Match(p.back(), pattern);
            });
            /**
             * filter empty folders
             */ 
            aux = Var::Trim(Logic::ForEach(aux, [](auto, auto v) {
                if(Var::IsInteger(v)) {
                    return (Var::Integer(v) == FileSystem::DIR)? Obj() : v;  
                }
                return v;
            }));
            /**
             * return a list of path files
             */
            return Logic::ForEach(
                Basic::GetKeys(Convert::ToSimpleMap(aux, "/")), [&path](auto, auto v) {
                    return Obj(String::Build(path, "/", Var::String(v)));
                }
            );
        }
        /**
         * --------------------------------------------------------------------
         * Not Found
         * --------------------------------------------------------------------
         */
        default : {
            return List{};
        }
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* FIND_H */