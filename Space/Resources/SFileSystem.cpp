/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   SFileSystem.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
/**
 * std
 */
#include <fstream>
#include <cstdio>
/**
 * space
 */
#include "SFileSystem.h"
#include "SConvert.h"
#include "SLogic.h"
#include "SSearch.h"
#include "SUtils.h"
#include "SKeys.h"
/**
 * namespaces
 */
using namespace std;
/**
 * --------------------------------------------------------------------------------------------------------------------
 * FileSystem Monitor
 * --------------------------------------------------------------------------------------------------------------------
 **/
const Integer SFileSystem::OPEN   = IN_OPEN;
const Integer SFileSystem::INPUT  = IN_MOVED_TO;
const Integer SFileSystem::CREATE = IN_CREATE;
const Integer SFileSystem::ACCESS = IN_ACCESS;
const Integer SFileSystem::MODIFY = IN_MODIFY;
const Integer SFileSystem::CLOSE  = IN_CLOSE;
const Integer SFileSystem::DELETE = IN_DELETE;
/**
 * ------------------------------------------------------------------------------------------------
 * utils
 * ------------------------------------------------------------------------------------------------
 */
static Boolean __contain(const Key& expr, const String& path) {
    static const regex e("([^/]+)");

    sregex_iterator eit(expr.begin(), expr.end(), e);
    sregex_iterator pit(path.begin(), path.end(), e);
    sregex_iterator end;

    for (; pit != end; ++eit, ++pit){
        if(eit == end){
            return false;
        }
        if (!regex_match(pit->str(), regex(eit->str()))) {
            return false;
        }
    }
    return true;
}
static List __paths_recursive(const String& path, const Key& expr) {
    return Utils::GetKeys(
        Search::Find(Obj(DT_DIR), 
            Logic::ForEach(SConvert::ToMap(SFileSystem::Find(path, expr), "/"), [](Var v){
                if(Var::IsMap(v)){
                    return Var(Obj(DT_DIR));
                }
                return v;
            })
        )
    );
}
static List __paths(const String& path, const Key& expr) {
    return Utils::GetKeys(SConvert::ToSimpleMap(SFileSystem::Find(path, expr), "/"));
}
static List __paths2(const String& path, const Key& expr) {
    return Utils::GetKeys(SConvert::ToMap(SFileSystem::Find(path, expr), "/"));
}
/**
 * ------------------------------------------------------------------------------------------------
 * File System
 * ------------------------------------------------------------------------------------------------
 */
SFileSystem::SFileSystem(const ::Map& watch) : SStream(), __watch_map(watch) {
    /**
     *  Create inotify instance
     */
    int fd = inotify_init();
    if (fd <= 0) {
        throw ResourceException(String::Build("inotify_init: ", strerror(errno)));
    }
    /**
     * register
     */
    for (auto& watch : __watch_map) {
        /**
         * is a pattern
         */
        if (Var::IsMap(watch.second)) {
            __watch_reg[
                inotify_add_watch(fd, watch.first.c_str(), IN_CREATE)
            ] = watch.first;
            for (auto pattern : Var::Map(watch.second)) {
                auto mask = int(Var(pattern.second));
                auto find = SFileSystem::Find(watch.first, pattern.first);
                auto smap = SConvert::ToSimpleMap(find, "/");
                auto cmap = SConvert::ToMap(find, "/");
                // add watchers 
                for (auto p : Utils::GetKeys(Edit::Delete(smap, cmap))) {
                    __watch_reg[
                        inotify_add_watch(
                            fd, (watch.first + "/" + Var::String(p)).c_str(), IN_CREATE
                        )
                    ] = (watch.first + "/" + Var::String(p));
                }
                for (auto p : Utils::GetKeys(smap)) {
                    __watch_reg[
                        inotify_add_watch(
                            fd, (watch.first + "/" + Var::String(p)).c_str(), mask
                        )
                    ] = (watch.first + "/" + Var::String(p));
                }
            } 
            continue;
        }
        /**
         * default
         */
        __watch_reg[
            inotify_add_watch(fd, watch.first.c_str(), int(Var(watch.second)))
        ] = watch.first;
    }
    /**
     * update resource
     */
    SStream::operator=(SStream(fd));
}

static Integer __watch_mask(const String& path, const Map& watch_map){
    for (auto& watch : watch_map) {
        if (Var::IsMap(watch.second)) {
            for (auto pattern : Var::Map(watch.second)) {
                if (__contain(watch.first + pattern.first, path)) {
                    return Var(pattern.second);
                }
            }
            continue;
        }
        if (__contain(watch.first, path)) {
            return Var(watch.second);
        }
    }
    return Integer();
}
List SFileSystem::read_events() {
    char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
    // read data
    int len = ::read(ihandler(), buf, sizeof (buf));
    if (len <= 0) {
        throw runtime_error(string("read: ") + strerror(errno));
    }
    // parse data
    List out;
    auto event = (const struct inotify_event *) buf;
    for (char * ptr = buf; ptr < buf + len; ptr += sizeof (struct inotify_event) + event->len) {
        event = (const struct inotify_event *) ptr;
        // create event structure
        String path = event->len ? __watch_reg[event->wd] + "/" + String(event->name) : __watch_reg[event->wd];
        // process event 
        if (event->mask & IN_CREATE) {
            if (event->mask & IN_ISDIR) {
                auto mask = __watch_mask(path, __watch_map);
                if (mask) {
                    __watch_reg[
                        inotify_add_watch(ihandler(), path.c_str(), mask)
                    ] = path;
                }
            }
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_CREATE)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_OPEN) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_OPEN)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_MOVED_TO) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_MOVED_TO)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_ACCESS) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_ACCESS)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_MODIFY) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_MODIFY)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_CLOSE) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_CLOSE)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_DELETE) {
            out.push_back(Obj{
                {_type_, Obj::Integer(IN_DELETE)},
                {_path_, Obj::String(path)},
            });
        }
        if (event->mask & IN_IGNORED) {
            if (event->len == 0) {
                __watch_reg.erase(event->wd);
                inotify_rm_watch(ihandler(), event->wd);
            }
        }
    }
    return out;
}

Integer SFileSystem::process_events(initializer_list<pair<const Integer, function<void(Var v)>>> l) {
    // map functions
    map<const Integer, function<void(Var v) >> functions(l);
    // read events
    List events = move(read_events());
    // process events
    for (Var e : events) functions[e[_type_]](e);
    // return number os events 
    return events.size();
}
/**
 * ------------------------------------------------------------------------------------------------
 * General operation over FileSystem
 * ------------------------------------------------------------------------------------------------
 * local interface
 * ----------------------------------------------------------------------------
 */
static Map __find_dir(sregex_iterator it, sregex_iterator& end, String path);
static Map __find_dir(String path);
static bool __copy_dir(String from, String to, Map tree);
static bool __copy_dir(String from, String to, List tree);
static bool __copy_file(String from, String to);
static bool __move_dir(String from, String to, Map tree);
static bool __move_dir(String from, String to, List tree);
static bool __move_file(String from, String to);
static bool __delete_dir(String path, Map tree);
static bool __delete_dir(String path, List tree);
static bool __delete_file(String path);
/**
 *
 */
Boolean SFileSystem::Insert(String path, Var tree) {
    if (Var::IsMap(tree)) {
        for (auto& e : Var::Map(tree)) {
            if (!Insert((path + e.first).data(), e.second)) {
                return false;
            }
        }
    } else if (Var::IsList(tree)) {
        for (auto& e : Var::List(tree)) {
            // create directory 
            auto r = ::mkdir((path + Var::ToString(e)).data(), S_IRWXU | S_IRWXG | S_IRWXO);
            // test make directory              
            if (r != 0 && r != EEXIST) {
                return false;
            }
        }
    } else {
        for(auto& e : SConvert::FromPath(Var::ToString(tree))){
            path += (Var::ToString(e) + "/");
            // create directory
            auto r = ::mkdir(path.data(), S_IRWXU | S_IRWXG | S_IRWXO);
            // test make directory              
            if (r < 0 && errno != EEXIST) {
                return false;
            }
        }
    }
    return true;
}

::Map SFileSystem::Properties(String path){
    Map out;
    struct stat st;
    if(stat((path).data(), &st) < 0) {
        throw runtime_error(strerror(errno));
    }
    // get type
    if (st.st_mode && S_IFDIR) {
        out[_type_] = Obj::Integer(DT_DIR);
    } else if (st.st_mode && S_IFREG) {
        out[_type_] = Obj::Integer(DT_REG);
    }
    // get size
    out[_size_] = Obj::Integer(st.st_size);
    // get modification time
    out[_time_] = Obj::Float(st.st_mtim.tv_sec);
    return out;
}

Var SFileSystem::Find(String path) {
    return Obj(__find_dir(path));
}

Var SFileSystem::Find(String path, Key expr) {
    static const regex e("([^/]+)");
    sregex_iterator it(expr.begin(), expr.end(), e), end;
    return Obj(__find_dir(it, end, path.back() == '/' ? path : path + "/"));
}

Boolean SFileSystem::Copy(String from, String to, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__copy_file(from , to)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__copy_dir(from, to, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__copy_dir(from, to, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__copy_file(from + Var::String(tree), to + Var::String(tree))) {
            return false;
        }
    }
    return true;
}

Boolean SFileSystem::Move(String from, String to, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__move_file(from , to)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__move_dir(from, to, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__move_dir(from, to, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__move_file(from + Var::String(tree), to + Var::String(tree))) {
            return false;
        }
    }
    return true;
}

Boolean SFileSystem::Delete(String path, Var tree) {
    if(Var::IsUndefined(tree)){
        if (!__delete_file(path)) {
            return false;
        }
    } else if (Var::IsMap(tree)) {
        if (!__delete_dir(path, Var::Map(tree))) {
            return false;
        }
    } else if (Var::IsList(tree)) {
        if (!__delete_dir(path, Var::List(tree))) {
            return false;
        }
    } else if (Var::IsString(tree)) {
        if (!__delete_file(path + "/" + Var::String(tree))) {
            return false;
        }
    } 
    return true;
}

String SFileSystem::GetFileName(String path) {
    if (path.back() == '/') {
        return String();
    }
    return Var::ToString(SConvert::FromPath(path).back());
}

String SFileSystem::GetFilePath(String path) {
    if (path.back() == '/')  return path;
    // convert to "[a, b, c]"
    auto tmp = SConvert::FromPath(path);
    // remove filename
    tmp.pop_back();
    // convert to "/a/b"
    return SConvert::ToPath(tmp);
}

String SFileSystem::GetFullPath(String path) {
    char tmp[FILENAME_MAX];
    if (realpath(path.data(), tmp) == 0) {
        throw runtime_error(strerror(errno));
    }
    return String(tmp);
}

String SFileSystem::GetPath() {
    char tmp[FILENAME_MAX];
    if (getcwd(tmp, FILENAME_MAX) == 0) {
        throw runtime_error(strerror(errno));
    }
    return String(tmp);
}

void SFileSystem::SetPath(String path) {
    if (chdir(path.data()) != 0) {
        throw runtime_error(strerror(errno));
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * local implementation
 * ------------------------------------------------------------------------------------------------
 */
static Map __find_dir(String path) {
    Map tree;
    /**
     * open directory
     */
    DIR* dir = ::opendir(path.data());
    if (dir == nullptr) {
        return tree;
    }
    /**
     * parse directory
     */
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        switch (entry->d_type) {
            case DT_DIR:
            {
                if (entry->d_name[0] == '.') {
                    break;
                }
                Map found(__find_dir(path + "/" + entry->d_name));
                if (found.empty()) {
                    tree[entry->d_name] = Obj(entry->d_type);
                } else {
                    tree[entry->d_name] = Obj(found);
                }
                break;
            }
            case DT_REG:
            {
                tree[entry->d_name] = Obj(entry->d_type);
                break;
            }
            case DT_LNK:
            {
                tree[entry->d_name] = Obj(entry->d_type);
                break;
            }
        }
    }
    /**
     * close directory
     */
    closedir(dir);
    return tree;
}

static Map __find_dir(sregex_iterator it, sregex_iterator& end, String path) {
    Map tree;
    /**
     * end search
     */
    if (it == end) {
        return tree;
    }
    String name(it->str());
    ++it;
    /**
     * try to get status
     */
    struct stat sb;
    if (stat((path + name).c_str(), &sb) != -1) {
        switch (sb.st_mode & S_IFMT) {
            case S_IFDIR:
            {
                Map found(__find_dir(it, end, path + name + "/"));
                if (found.empty()) {
                    tree[name] = Obj(DT_DIR);
                } else {
                    tree[name] = Obj(found);
                }
                break;
            }
            case S_IFLNK:
            {
                tree[name] = Obj(DT_LNK);
                break;
            }
            case S_IFREG:
            {
                tree[name] = Obj(DT_REG);
                break;
            }
            default:;
        }
        return tree;
    }
    /**
     * open directory
     */
    DIR* dir = ::opendir(path.data());
    if (dir == nullptr) {
        return tree;
    }
    /**
     * parse directory and match expression
     */
    regex expr(name);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!regex_match(entry->d_name, expr)) {
            continue;
        }
        switch (entry->d_type) {
            case DT_DIR:
            {
                if (entry->d_name[0] == '.') {
                    break;
                }
                Map found(__find_dir(it, end, path + entry->d_name + "/"));
                if (found.empty()) {
                    tree[entry->d_name] = Obj(DT_DIR);
                } else {
                    tree[entry->d_name] = Obj(found);
                }
                break;
            }
            case DT_REG:
            {
                tree[entry->d_name] = Obj(DT_REG);
                break;
            }
            case DT_LNK:
            {
                tree[entry->d_name] = Obj(DT_LNK);
                break;
            }
        }
    }
    /**
     * close directory
     */
    closedir(dir);
    return tree;
}

static bool __copy_dir(String from, String to, Map tree) {
    ::mkdir(to.data(), S_IRWXU | S_IRWXG | S_IRWXO);
    for (auto& e : tree) {
        if (Var::IsMap(e.second)) {
            if (!__copy_dir(from + "/" + e.first, to + "/" + e.first, Var::Map(e.second))) {
                return false;
            }
        } else if (Var::IsInteger(e.second)) {

            switch (Var::Integer(e.second)) {
                case DT_LNK:
                case DT_REG:
                {
                    if (!__copy_file(from + "/" + e.first, to + "/" + e.first)) {
                        return false;
                    }
                    break;
                }
                case DT_DIR:
                {
                    if (::access((from + "/" + e.first).data(), F_OK) != 0) {
                        return false;
                    }
                    ::mkdir((to + "/" + e.first).data(), S_IRWXU | S_IRWXG | S_IRWXO);
                    break;
                }
            }
        }
    }
    return true;
}

static bool __copy_dir(String from, String to, List tree) {
    ::mkdir(to.data(), S_IRWXU | S_IRWXG | S_IRWXO);
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__copy_dir(from, to, Var::Map(e))) {
                return false;
            }
        } else if (Var::IsList(e)) {
            if (!__copy_dir(from, to, Var::List(e))) {
                return false;
            }
        } else if (Var::IsString(e)) {
            if (!__copy_file(from + "/" + Var::String(e), to + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}

static bool __copy_file(String from, String to) {
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

static bool __move_dir(String from, String to, Map tree) {
    ::mkdir(to.data(), S_IRWXU | S_IRWXG | S_IRWXO);
    for (auto& e : tree) {
        if (Var::IsMap(e.second)) {
            if (!__move_dir(from + "/" + e.first, to + "/" + e.first, Var::Map(e.second))) {
                return false;
            }
        } else if (Var::IsInteger(e.second)) {

            switch (Var::Integer(e.second)) {
                case DT_LNK:
                case DT_REG:
                {
                    if (!__move_file(from + "/" + e.first, to + "/" + e.first)) {
                        return false;
                    }
                    break;
                }
                case DT_DIR:
                {
                    if (::access((from + "/" + e.first).data(), F_OK) != 0) {
                        return false;
                    }
                    ::mkdir((to + "/" + e.first).data(), S_IRWXU | S_IRWXG | S_IRWXO);
                    break;
                }
            }
        }
    }
    return true;
}

static bool __move_dir(String from, String to, List tree) {
    ::mkdir(to.data(), S_IRWXU | S_IRWXG | S_IRWXO);
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__move_dir(from, to, Var::Map(e))) {
                return false;
            }
        } else if (Var::IsList(e)) {
            if (!__move_dir(from, to, Var::List(e))) {
                return false;
            }
        } else if (Var::IsString(e)) {
            if (!__move_file(from + "/" + Var::String(e), to + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}

static bool __move_file(String from, String to) {
    unlink(to.data());

    if (link(from.data(), to.data()) != 0) {
        return false;
    }
    if (unlink(from.data()) != 0) {
        return false;
    }
    return true;
}

static bool __delete_dir(String path, Map tree) {
    for (auto& e : tree) {
        if (Var::IsMap(e.second)) {
            if (!__delete_dir(path + "/" + e.first, Var::Map(e.second))) {
                return false;
            }
            ::rmdir(String::Build(path, "/", e.first).data());
        } else if (Var::IsList(e.second)) {
            if (!__delete_dir(path + "/" + e.first, Var::List(e.second))) {
                return false;
            }
            ::rmdir(String::Build(path, "/", e.first).data());
        } else if (Var::IsString(e.second)) {
            if (!__delete_file(path + "/" + Var::String(e.second))) {
                return false;
            }
        } else if (Var::IsInteger(e.second)) {
            if (!__delete_file(path + "/" + e.first)) {
                return false;
            }
        }
    }
    return true;
}

static bool __delete_dir(String path, List tree) {
    for (Var e : tree) {
        if (Var::IsMap(e)) {
            if (!__delete_dir(path, Var::Map(e))) {
                return false;
            }
            ::rmdir(path.data());
        } else if (Var::IsList(e)) {
            if (!__delete_dir(path, Var::List(e))) {
                return false;
            }
            ::rmdir(path.data());
        } else if (Var::IsString(e)) {
            if (!__delete_file(path + "/" + Var::String(e))) {
                return false;
            }
        }
    }
    return true;
}

static bool __delete_file(String file) {
    return (remove(file.data()) == 0);
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */