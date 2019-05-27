/**
 * ------------------------------------------------------------------------------------------------ 
 * File:   SFileSystem.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
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
/**
 * -------------------------------------------------------------------------------------------------
 *  File System
 * ------------------------------------------------------------------------------------------------
 */
namespace FileSystem {
    /**
     * ------------------------------------------------------------------------
     * Create file tree
     * @param path
     * @param tree
     * @return 
     * ------------------------------------------------------------------------
     */
    static Boolean Insert(String path, Var tree = Obj::Null());
    /**
     * ------------------------------------------------------------------------
     * Delete file tree
     * @param path
     * @param tree
     * @return 
     * ------------------------------------------------------------------------
     */
    static Boolean Delete(String path, Var tree = Obj::Null());
    /**
     * ------------------------------------------------------------------------
     * Find file tree
     * @param path
     * @param filter function
     * @return tree
     * ------------------------------------------------------------------------
     */
    Var Find(String path, std::function<bool(const KeyList&, int)> filter);
    /**
     * ------------------------------------------------------------------------
     * Copy Files 
     * @param from
     * @param to
     * @param tree
     * @return
     * ------------------------------------------------------------------------ 
     */
    static Boolean Copy(
        String from, String to, Var tree = Obj::Null());
    /**
     * ------------------------------------------------------------------------
     * Move Files 
     * @param from
     * @param to 
     * @param tree
     * ------------------------------------------------------------------------
     */
    static Boolean Move(
        String from, String to, Var tree = Obj::Null());
    /**
     * ------------------------------------------------------------------------
     * Get path from full path
     * ------------------------------------------------------------------------
     */
    static String GetFileName(String path);
    /**
     * ------------------------------------------------------------------------
     * Get File Name from full path
     * ------------------------------------------------------------------------
     */
    static String GetFilePath(String path);
    /**
     * ------------------------------------------------------------------------
     * Get get path from root
     * ------------------------------------------------------------------------
     */
    static String GetFullPath(String path);
    /**
     * ------------------------------------------------------------------------
     * Get working directory
     * ------------------------------------------------------------------------
     */
    static String GetPath();
    /**
     * ------------------------------------------------------------------------
     * Set working directory
     * ------------------------------------------------------------------------
     */
    static void SetPath(String path);
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SFILESYSTEM_H */

