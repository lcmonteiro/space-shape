/**
 * ------------------------------------------------------------------------------------------------ 
 * File:   SFileSystem.h
 * Author: Luis Monteiro
 *
 * Created on October 7, 2015, 11:25 AM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SFILESYSTEM_H
#define SFILESYSTEM_H
/**
 * std
 */
#include <functional>
/**
 * space
 */
#include "SVariable.h"
#include "SStream.h"
/**
 * -------------------------------------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------------------------------------
 */
class SFileSystem: public SStream {
public:
    /**
     * ------------------------------------------------------------------------
     * FileSystem Monitor
     * ------------------------------------------------------------------------
     */
    static const Integer OPEN;
    static const Integer INPUT;
    static const Integer CREATE;
    static const Integer ACCESS;
    static const Integer MODIFY;
    static const Integer CLOSE;
    static const Integer DELETE;
    /**
     * ----------------------------------------------------
     * constructors
     * ----------------------------------------------------
     */
    SFileSystem(const ::Map& watch);
    // empty
    SFileSystem() = default;
    // move
    SFileSystem(SFileSystem&&) = default;
    /**
     * ----------------------------------------------------
     * move operatore
     * ----------------------------------------------------
     */
    SFileSystem& operator=(SFileSystem&&) = default;
    /**
     * ----------------------------------------------------
     * read events
     * ----------------------------------------------------
     */
    ::List read_events();
    /**
     * ----------------------------------------------------
     * process events
     * ----------------------------------------------------
     */
    Integer process_events(
        std::initializer_list<
            std::pair<const Integer, std::function<void(Var v)>>
        > l
    );
    /**
     * ------------------------------------------------------------------------
     * General operation over FileSystem
     * ------------------------------------------------------------------------
     * Create file tree
     * @param path
     * @param tree
     * @return 
     * ----------------------------------------------------
     */
    static Boolean Insert(String path, Var tree=Obj::Null());

    /**
     * ----------------------------------------------------
     * Delete file tree
     * @param path
     * @param tree
     * @return 
     * ----------------------------------------------------
     */
    static Boolean Delete(String path, Var tree=Obj::Null());
    
    /**
     * ----------------------------------------------------
     * Find file tree
     * @param path
     * @return tree
     * ----------------------------------------------------
     */
    static Var Find(String path, Key expr);
    static Var Find(String path);
    
    /**
     * ----------------------------------------------------
     * Properties of resource
     * ----------------------------------------------------
     */
    static ::Map Properties(String path);

    /**
     * ----------------------------------------------------
     * Copy Files 
     * @param from
     * @param to
     * @param tree
     * @return
     * ---------------------------------------------------- 
     */
    static Boolean Copy(String from, String to, Var tree=Obj::Null());
    
    /**
     * ----------------------------------------------------
     * Move Files 
     * @param from
     * @param to 
     * @param tree
     * ----------------------------------------------------
     */
    static Boolean Move(String from, String to, Var tree=Obj::Null());
    
    /**
     * ----------------------------------------------------
     * Get path from full path
     * ----------------------------------------------------
     */
    static String GetFileName(String path);

    inline static Var GetFileName(Var path) {
        return Obj(GetFileName(String(path)));
    }
    /**
     * ----------------------------------------------------
     * Get File Name from full path
     * ----------------------------------------------------
     */
    static String GetFilePath(String path);
    
    inline static Var GetFilePath(Var path) {
        return Obj(GetFilePath(Var::ToString(path)));
    }
    /**
     * ----------------------------------------------------
     * Get get path from root
     * ----------------------------------------------------
     */
    static String GetFullPath(String path);
    /**
     * ----------------------------------------------------
     * Get System directory
     * ----------------------------------------------------
     */
    static String GetPath();
protected:
    /**
     * ----------------------------------------------------
     * set working directory
     * ----------------------------------------------------
     */
    static void SetPath(String path);
private:
    /**
     * ----------------------------------------------------
     * map of files or directories registered
     * ----------------------------------------------------
     * path to mask
     */
    Map __watch_map;
    /**
     * fd to path
     */
    std::map<Integer, String> __watch_reg;
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SFILESYSTEM_H */

