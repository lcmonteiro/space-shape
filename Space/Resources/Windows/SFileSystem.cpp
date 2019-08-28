/**
 * ------------------------------------------------------------------------------------------------ 
 * File:   FileSystem.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/**
 * std
 */
#include <fstream>
/**
 * space
 */
#include "SFileSystem.h"
#include "SConvert.h"
/**
 * namespaces
 */
using namespace std;
/**
 * ------------------------------------------------------------------------------------------------
 * Local Variables
 * ------------------------------------------------------------------------------------------------
 */
// static map<int, FileSystem::Resource> __map {
//     {DT_DIR, FileSystem::DIR},
//     {DT_REG, FileSystem::FILE},
//     {DT_LNK, FileSystem::LINK}
// };
/**
 * ------------------------------------------------------------------------------------------------
 * Local Interface
 * ------------------------------------------------------------------------------------------------
 */
static Map  __FindDirectory(KeyList& path, std::function<bool(const KeyList&, int)> filter);

static bool __CopyDirectory(String from, String to, Map tree);
static bool __CopyDirectory(String from, String to, List tree);
static bool __CopyFile(String from, String to);

static bool __MoveDirectory(String from, String to, Map tree);
static bool __MoveDirectory(String from, String to, List tree);
static bool __MoveFile(String from, String to);

static bool __DeleteDirectory(String path, Map tree);
static bool __DeleteDirectory(String path, List tree);
static bool __DeleteFile(String path);
/**
 * ----------------------------------------------------------------------------
 * Insert Folder Tree 
 * ----------------------------------------------------------------------------
 */
Boolean FileSystem::Insert(String path, Var tree) {
    if (Var::IsMap(tree)) {
        for (auto& e : Var::Map(tree)) {
            if (!Insert((path + e.first).data(), e.second)) {
                return false;
            }
        }
    } else if (Var::IsList(tree)) {
        for (auto& e : Var::List(tree)) {
            // create directory 
            auto r = _mkdir((path + Var::ToString(e)).data());
            // test make directory              
            if (r != 0 && r != EEXIST) {
                return false;
            }
        }
    } else {
        for(auto& e : Convert::FromPath(Var::ToString(tree))){
            path += (Var::ToString(e) + "/");
            // create directory
            auto r = _mkdir(path.data());
            // test make directory              
            if (r < 0 && errno != EEXIST) {
                return false;
            }
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * File Resources on Folder Tree
 * ----------------------------------------------------------------------------
 */
Var FileSystem::Find(String path, function<bool(const KeyList&, int)> filter) {
    auto stack = KeyList{path};
    return Obj(__FindDirectory(stack, filter));
}
/**
 * ----------------------------------------------------------------------------
 * Copy Folder Tree
 * ----------------------------------------------------------------------------
 */
Boolean FileSystem::Copy(String from, String to, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__CopyFile(from , to)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__CopyDirectory(from, to, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__CopyDirectory(from, to, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__CopyFile(from + Var::String(tree), to + Var::String(tree))) {
            return false;
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Move Folder Tree
 * ----------------------------------------------------------------------------
 */
Boolean FileSystem::Move(String from, String to, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__MoveFile(from , to)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__MoveDirectory(from, to, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__MoveDirectory(from, to, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__MoveFile(from + Var::String(tree), to + Var::String(tree))) {
            return false;
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Delete Folder Tree
 * ----------------------------------------------------------------------------
 */
Boolean FileSystem::Delete(String path, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__DeleteFile(path)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__DeleteDirectory(path, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__DeleteDirectory(path, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__DeleteFile(path + "/" + Var::String(tree))) {
            return false;
        }
    } 
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Get File Name
 * ----------------------------------------------------------------------------
 */
String FileSystem::GetFileName(String path) {
    if (path.back() == '/') {
        return String();
    }
    return Var::ToString(Convert::FromPath(path).back());
}
/**
 * ----------------------------------------------------------------------------
 *  Get File Path
 * ----------------------------------------------------------------------------
 */
String FileSystem::GetFilePath(const String& path) {
    if (path.back() == '/')  return path;
    // convert to "[a, b, c]"
    auto tmp = Convert::FromPath(path);
    // remove filename
    tmp.pop_back();
    // convert to "/a/b"
    return Convert::ToPath(tmp);
}
/**
 * ----------------------------------------------------------------------------
 * Get Full File Path
 * ----------------------------------------------------------------------------
 */
String FileSystem::GetFullPath(const String& path) {
    char tmp[_MAX_PATH];
    if(_fullpath(tmp, path.data(), _MAX_PATH ) == NULL) { 
        throw runtime_error(strerror(errno));
    }    
    return String(tmp);
}
/**
 * ----------------------------------------------------------------------------
 * Get File Extension
 * ----------------------------------------------------------------------------
 */
String FileSystem::GetExtension(const String& path) {
    // get file name
    auto name = GetFileName(path);
    // get extension
    return name.substr(name.find_last_of(".") + 1);
}
/**
 * ----------------------------------------------------------------------------
 * Get Working Directory
 * ----------------------------------------------------------------------------
 */
FileSystem::Resource FileSystem::GetPathType(const String& path) {
    struct _stat st;
    if(_stat(path.data(), &st) >= 0) {
        if (st.st_mode & _S_IFDIR) {
            return Resource::DIR;
        } else if (st.st_mode & _S_IFREG) {
            return Resource::FILE;
        }
    }
    return Resource::NONE;
}
/**
 * ----------------------------------------------------------------------------
 * Get Working Directory
 * ----------------------------------------------------------------------------
 */
String FileSystem::GetPath() {
    char tmp[FILENAME_MAX];
    if (getcwd(tmp, FILENAME_MAX) == 0) {
        throw runtime_error(strerror(errno));
    }
    return String(tmp);
}
/**
 * ----------------------------------------------------------------------------
 * Set Working Directory
 * ----------------------------------------------------------------------------
 */
void FileSystem::SetPath(String path) {
    if (chdir(path.data()) != 0) {
        throw runtime_error(strerror(errno));
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * Local Implementation
 * ------------------------------------------------------------------------------------------------
 * Find Directories
 * ----------------------------------------------------------------------------
 */
static Map __FindDirectory(KeyList& path, function<bool(const KeyList&, int)> filter) {
    Map tree;
    // /**
    //  * open directory
    //  */
    // DIR* dir = ::opendir(Convert::ToPath(path).data());
    // if (dir == nullptr) {
    //     return tree;
    // }
    // /**
    //  * parse directory
    //  */
    // struct dirent *entry;
    // while ((entry = readdir(dir)) != NULL) {
    //     path.emplace_back(entry->d_name);
    //     switch (entry->d_type) {
    //         case DT_DIR:
    //         {
    //             if (entry->d_name[0] == '.') {
    //                 break;
    //             }
    //             if(filter(path, __map[DT_DIR])) {
    //                 auto found = __FindDirectory(path, filter);
    //                 if (found.empty()) {
    //                     tree[entry->d_name] = Obj(__map[entry->d_type]);
    //                 } else {
    //                     tree[entry->d_name] = Obj(found);
    //                 }
    //             }
    //             break;
    //         }
    //         case DT_REG:
    //         {
    //             if(filter(path, __map[DT_REG])) {
    //                 tree[entry->d_name] = Obj(__map[entry->d_type]);
    //             }
    //             break;
    //         }
    //         case DT_LNK:
    //         {
    //             if(filter(path, __map[DT_LNK])) {
    //                 tree[entry->d_name] = Obj(__map[entry->d_type]);
    //             }
    //             break;
    //         }
    //     }
    //     path.pop_back();
    // }
    // /**
    //  * close directory
    //  */
    // closedir(dir);
    return tree;
}
/**
 * ----------------------------------------------------------------------------
 * Copy Directories
 * ----------------------------------------------------------------------------
 */
static bool __CopyDirectory(String from, String to, Map tree) {
    _mkdir(to.data());
    // for (auto& e : tree) {
    //     if (Var::IsMap(e.second)) {
    //         if (!__CopyDirectory(from + "/" + e.first, to + "/" + e.first, Var::Map(e.second))) {
    //             return false;
    //         }
    //     } else if (Var::IsInteger(e.second)) {

    //         switch (Var::Integer(e.second)) {
    //             case DT_LNK:
    //             case DT_REG:
    //             {
    //                 if (!__CopyFile(from + "/" + e.first, to + "/" + e.first)) {
    //                     return false;
    //                 }
    //                 break;
    //             }
    //             case DT_DIR:
    //             {
    //                 if (::access((from + "/" + e.first).data(), F_OK) != 0) {
    //                     return false;
    //                 }
    //                 _mkdir((to + "/" + e.first).data());
    //                 break;
    //             }
    //         }
    //     }
    // }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Copy Directories
 * ----------------------------------------------------------------------------
 */
static bool __CopyDirectory(String from, String to, List tree) {
    _mkdir(to.data());
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__CopyDirectory(from, to, Var::Map(e))) {
                return false;
            }
        } else if (Var::IsList(e)) {
            if (!__CopyDirectory(from, to, Var::List(e))) {
                return false;
            }
        } else if (Var::IsString(e)) {
            if (!__CopyFile(from + "/" + Var::String(e), to + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Copy File
 * ----------------------------------------------------------------------------
 */
static bool __CopyFile(String from, String to) {
    ifstream fromf(from, ios::binary);
    if (fromf.fail()) {
        return false;
    }
    ofstream tof(to, ios::binary);
    if (tof.fail()) {
        return false;
    }
    tof << fromf.rdbuf() << flush;
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Move Directory
 * ----------------------------------------------------------------------------
 */
static bool __MoveDirectory(String from, String to, Map tree) {
    _mkdir(to.data());
    // for (auto& e : tree) {
    //     if (Var::IsMap(e.second)) {
    //         if (!__MoveDirectory(from + "/" + e.first, to + "/" + e.first, Var::Map(e.second))) {
    //             return false;
    //         }
    //     } else if (Var::IsInteger(e.second)) {

    //         switch (Var::Integer(e.second)) {
    //             case DT_LNK:
    //             case DT_REG:
    //             {
    //                 if (!__MoveFile(from + "/" + e.first, to + "/" + e.first)) {
    //                     return false;
    //                 }
    //                 break;
    //             }
    //             case DT_DIR:
    //             {
    //                 if (::access((from + "/" + e.first).data(), F_OK) != 0) {
    //                     return false;
    //                 }
    //                 _mkdir((to + "/" + e.first).data());
    //                 break;
    //             }
    //         }
    //     }
    // }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Move Directory
 * ----------------------------------------------------------------------------
 */
static bool __MoveDirectory(String from, String to, List tree) {
    _mkdir(to.data());
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__MoveDirectory(from, to, Var::Map(e))) {
                return false;
            }
        } else if (Var::IsList(e)) {
            if (!__MoveDirectory(from, to, Var::List(e))) {
                return false;
            }
        } else if (Var::IsString(e)) {
            if (!__MoveFile(from + "/" + Var::String(e), to + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Move File
 * ----------------------------------------------------------------------------
 */
static bool __MoveFile(String from, String to) {
    return std::rename(from.data(), to.data());
}
/**
 * ----------------------------------------------------------------------------
 * Delete Directory
 * ----------------------------------------------------------------------------
 */
static bool __DeleteDirectory(String path, Map tree) {
    for (auto& e : tree) {
        if (Var::IsMap(e.second)) {
            if (!__DeleteDirectory(path + "/" + e.first, Var::Map(e.second))) {
                return false;
            }
            ::rmdir(String::Build(path, "/", e.first).data());
        } else if (Var::IsList(e.second)) {
            if (!__DeleteDirectory(path + "/" + e.first, Var::List(e.second))) {
                return false;
            }
            ::rmdir(String::Build(path, "/", e.first).data());
        } else if (Var::IsString(e.second)) {
            if (!__DeleteFile(path + "/" + Var::String(e.second))) {
                return false;
            }
        } else if (Var::IsInteger(e.second)) {
            if (!__DeleteFile(path + "/" + e.first)) {
                return false;
            }
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Delete Directory
 * ----------------------------------------------------------------------------
 */
static bool __DeleteDirectory(String path, List tree) {
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__DeleteDirectory(path, Var::Map(e))) {
                return false;
            }
            ::rmdir(path.data());
        } else if (Var::IsList(e)) {
            if (!__DeleteDirectory(path, Var::List(e))) {
                return false;
            }
            ::rmdir(path.data());
        } else if (Var::IsString(e)) {
            if (!__DeleteFile(path + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Delete File
 * ----------------------------------------------------------------------------
 */
static bool __DeleteFile(String file) {
    return (remove(file.data()) == 0);
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */