/**
 * ------------------------------------------------------------------------------------------------
 * File:   File.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 10:12 AM
 * ------------------------------------------------------------------------------------------------
 */
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
/**
 * std
 */
#include <cstring>
#include <stdexcept>
/**
 * space
 */
#include "SFile.h"
/**
 * ----------------------------------------------------------------------------
 * File utilities 
 * ----------------------------------------------------------------------------
 */
bool SAdapter::SFile::Exist(int fd) {
    struct stat st;
    if (fstat(fd, &st) < 0) {
        throw ResourceException(strerror(errno));
    }
    return st.st_nlink > 0;
}
/**
 * ----------------------------------------------------------------------------
 * Open File Types
 * ----------------------------------------------------------------------------
 */
int SAdapter::SFileAppend::Open(const String& path) {
    int fd = -1;
    if ((fd = open(path.data(), O_CREAT | O_APPEND | O_RDWR, 0)) < 0) {
        throw ResourceException(strerror(errno));
    }
    return fd;
}
int SAdapter::SFileTrunc::Open(const String& path) {
    int fd = -1;
    if ((fd = open(path.data(), O_CREAT | O_TRUNC | O_RDWR, 0)) < 0) {
        throw ResourceException(strerror(errno));
    }
    return fd;
}
int SAdapter::SFileRead::Open(const String& path) {
    int fd = -1;
    if ((fd = open(path.data(), O_RDONLY, 0)) < 0) {
        throw ResourceException(strerror(errno));
    }
    return fd;
}
int SAdapter::SFileNormal::Open(const String& path) {
    int fd = -1;
    if ((fd = open(path.data(), O_CREAT | O_RDWR, 0)) < 0) {
        throw ResourceException(strerror(errno));
    }
    return fd;
}
/**
 * ----------------------------------------------------------------------------
 * logger
 * ----------------------------------------------------------------------------
 */
bool SLogger::__overload() {
    return lseek(ihandler(), 0, SEEK_CUR) > _size;
}
void SLogger::__rotate() {
    for (Integer i = _n - 1, j = _n; i > 0; --i, --j) {
        /** 
         * rename backlog files
         */
        ::unlink(
            String::build(path(), ".", String::ValueOf(j)).c_str()
        );
        ::link(
            String::build(path(), ".", String::ValueOf(i)).c_str(), 
            String::build(path(), ".", String::ValueOf(j)).c_str()
        );
    }
    /** 
     * rename current file
     */
    ::unlink(
        String::build(path(), ".", String::ValueOf(Integer(1))).c_str()
    );
    ::linkat(
        ihandler(), "", AT_FDCWD,
        String::build(path(), ".", String::ValueOf(Integer(1))).c_str(),
        AT_EMPTY_PATH
    );
    /**
     * unlink current
     */
    ::unlink(path().c_str());
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
