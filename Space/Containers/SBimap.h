/**
 * ------------------------------------------------------------------------------------------------
 * File:   Bimap.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef BIMAP_H
#define	BIMAP_H
/**
 * std
 */
#include <map>
/**
 * space
 */
#include "Variables.h"
/**
 * -----------------------------------------------------------------------------------------------
 * Bimap
 * -----------------------------------------------------------------------------------------------
 */
template <
        typename T1, 
        typename T2, 
        typename Compare1 = std::less<T1>, 
        typename Compare2 = std::less<T2>
>
class Bimap {
public:
    typedef map<T1, T2, Compare1> map1;
    typedef map<T2, T1, Compare2> map2;
    /**
     * constructors
     */
    Bimap()               = default;
    Bimap(const Bimap& v) = default;
    Bimap(Bimap&& v)      = default;
    /**
     * operators
     */
    Bimap& operator=(const Bimap&) = default;
    Bimap& operator=(Bimap&&) = default;
    /**
     * quantity
     */
    bool empty() {
        return _1.empty(); 
    }
    size_t size(){
        return _1.size(); 
    }
    size_t count(T1 v1) {
        return _1.count(v1); 
    }
    size_t rcount(T2 v2) {
        return _2.count(v2); 
    }
    /**
     * insert
     */
    Bimap& insert(T1 v1, T2 v2){
        _1[v1] = v2;
        _2[v2] = v1;
        return *this;
    }
    /**
     * erase
     */
    Bimap& erase(T1 v1){
        _2.erase(_1.at(v1));
        _1.erase(v1);
        return *this;
    }
    Bimap& rerase(T2 v2){
        _1.erase(_2.at(v2));
        _2.erase(v2);
        return *this;
    }
    /**
     * find
     */
    T2 find(T1 v1){
        return _1.at(v1);
    }
    T1 rfind(T2 v2){
        return _2.at(v2);
    }
    /**
     * std iterators
     */
    typename map1::const_iterator cbegin() {
        return _1.cbegin();
    }
    typename map1::iterator begin() {
        return _1.begin();
    }
    typename map1::iterator erase(typename map1::iterator* it) {
        _2.erase(it->second);
        return _1.erase(it);
    }
    typename map1::iterator end() {
        return _1.end();
    }
    typename map1::const_iterator cend() {
        return _1.cend();
    }
    /**
     * reverse iterartors
     */
    typename map2::const_iterator crbegin() {
        return _1.cbegin();
    }
    typename map2::iterator rbegin() {
        return _2.begin();
    }
    typename map2::iterator rerase(typename map2::iterator* it) {
        _1.erase(it->second);
        return _2.erase(it);
    }
    typename map2::iterator rend() {
        return _2.end();
    }
    typename map2::const_iterator crend() {
        return _2.cend();
    }
protected:
    map1 _1;
    map2 _2;
};
/**
 * ------------------------------------------------------------------------------------------------
 * BimapKey
 * ------------------------------------------------------------------------------------------------
 */
typedef class BimapKey : public Bimap<Key, Key> {
public:
    /**
     * Base
     */
    using Bimap<Key, Key>::Bimap;
    using Bimap<Key, Key>::operator=;
    /**
     * BimapKey from Map
     */
    BimapKey(const Map& data){
        for(auto& v : data){
            insert(v.first, Var::String(v.second));
        }
    }
} BimapKey;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* BIMAP_H */

