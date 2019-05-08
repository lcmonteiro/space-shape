/* 
 * File:   SCsvConvert.cpp
 * Author: João Lima
 *
 * Created on Apr 10, 2019, 12:11 PM
 */
#include <sstream>
#include <iostream>
/**
 */
#include "Stypes/SVariables.h"
#include "Stools/SUtils.h"
/**
 */
#include "SConvertCSV.h"
/**
 */
Var __Scsvconvert__::fromCSV(std::istream& v, char delim){
        /**
         * Parse CSV
         */
        try{
                List lines;
                for(String line; std::getline(v, line);) {
                        List values;
                        /**
                         * get data of each cell
                         */
                        stringstream s(line.update('\r', '\0'));
                        String value;
                        for (; std::getline(s, value, delim);) {
                                values.push_back(Obj(value));
                        }
                        if(!value.empty()){
                                values.push_back(Obj(value));
                        }
                        /**
                         * save line
                         */
                        lines.push_back(Obj(values));
                }
                if(lines.empty()){
                        return Obj::Null();
                }
                /**
                 * create key:value
                 */
                List::iterator it = lines.begin();
                for (++it; it != lines.end(); ++it) {
                        *it = Obj(Utils::combine(Var::List(lines.front()), Var::List(*it)));
                }
                /**
                 * delete first item
                 */
                lines.erase(lines.begin());
                /**
                 */
                return Obj(lines);
        } catch(...) {
                return Obj::Null();
        }
}

std::ostream& __Scsvconvert__::toCSV(std::ostream& os, Var data, char delim){
        /**
         * convert to list
         */  
        List entries = Logic::toList(data);
        /**
         * find keys
         */
        List keys;
        auto it = entries.begin();
        if (it != entries.end()) {
                if(Var::IsMap(*it)){
                        keys = Utils::getKeys(Var::Map(*it));
                        
                }
        }
        /**
         * insert keys
         */
        ostringstream los;
        for (auto key : keys) {
                los << Var::String(key) << delim;
        }
        os << String(los.str()).detete(1) << '\n';
        /**
         * insert entries
         */
        for (; it != entries.end(); ++it) {
                ostringstream los;
                if (Var::IsMap(*it)) {
                        Map& map = Var::Map(*it);
                        for (Var key : keys) {
                                try {
                                        los << Var::String(map.at(key)) << delim;
                                } catch(out_of_range& ex){
                                        los << "" << delim;
                                }
                        }
                }
                os << String(los.str()).detete(1) << '\n';
        }
        return os;
}
