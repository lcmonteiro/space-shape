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
    typedef enum {DIR = 0, FILE, LINK, NONE} Resource;
    /**
     * ------------------------------------------------------------------------
     * Create file tree
     * @param path
     * @param tree
     * @return 
     * ------------------------------------------------------------------------
     */
    Boolean Insert(String path, Var tree = Obj());
    /**
     * ------------------------------------------------------------------------
     * Delete file tree
     * @param path
     * @param tree
     * @return 
     * ------------------------------------------------------------------------
     */
    Boolean Delete(String path, Var tree = Obj());
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
    Boolean Copy(String from, String to, Var tree = Obj());
    /**
     * ------------------------------------------------------------------------
     * Move Files 
     * @param from
     * @param to 
     * @param tree
     * ------------------------------------------------------------------------
     */
    Boolean Move(String from, String to, Var tree = Obj());
    /**
     * ------------------------------------------------------------------------
     * Get File Name from full path
     * ------------------------------------------------------------------------
     */
    String GetFileName(String path);
    /**
     * ------------------------------------------------------------------------
     * Get File Path from full path
     * ------------------------------------------------------------------------
     */
    String GetFilePath(const String& path);
    /**
     * ------------------------------------------------------------------------
     * Get Path from root
     * ------------------------------------------------------------------------
     */
    String GetFullPath(const String& path);
    /**
     * ------------------------------------------------------------------------
     * Get File Extension
     * ------------------------------------------------------------------------
     */
    String GetExtension(const String& path);
    /**
     * ------------------------------------------------------------------------
     * Get Path Type
     * ------------------------------------------------------------------------
     */
    Resource GetPathType(const String& path);
    /**
     * ------------------------------------------------------------------------
     * Get working directory
     * ------------------------------------------------------------------------
     */
    String GetPath();
    /**
     * ------------------------------------------------------------------------
     * Set working directory
     * ------------------------------------------------------------------------
     */
    void SetPath(String path);
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SFILESYSTEM_H */

