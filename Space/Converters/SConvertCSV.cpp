/**
 * ------------------------------------------------------------------------------------------------ 
 * File:   SCsvConvert.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 * space
 */
#include "SConvertCSV.h"
#include "SUtils.h"
#include "SEdit.h"
/**
 * ----------------------------------------------------------------------------
 * From CSV
 * ----------------------------------------------------------------------------
 */
Var Convert::FromCSV(std::istream& v, char delim) {
    try{
        List lines;
        for(String line; std::getline(v, line);) {
            List values;
            /**
             * get data of each cell
             */
            stringstream s(line.update('\r', '\0'));
            String value;
            while (std::getline(s, value, delim)) {
                values.emplace_back(Obj(value));
            }
            if(!value.empty()){
                values.emplace_back(Obj(value));
            }
            /**
             * save line
             */
            lines.emplace_back(Obj(values));
        }
        if(lines.empty()){
            return Obj::Null();
        }
        /**
         * create key:value
         */
        List::iterator it = lines.begin();
        for (++it; it != lines.end(); ++it) {
            *it = Obj(Utils::Combine(
                Var::List(lines.front()), Var::List(*it)));
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
Var Convert::FromCSV(std::istream&& v, char delim) {
    return FromCSV(v, delim);
}
/**
 * ----------------------------------------------------------------------------
 * To CSV
 * ----------------------------------------------------------------------------
 */
std::ostream& Convert::ToCSV(std::ostream& os, Var data, char delim) {
    /**
     * convert to list
     */  
    List entries = Logic::ToList(data);
    /**
     * find keys
     */
    List keys;
    auto it = entries.begin();
    if (it != entries.end()) {
        if(Var::IsMap(*it)){
            keys = Utils::GetKeys(Var::Map(*it));
        }
    }
    /**
     * insert keys
     */
    ostringstream los;
    for (auto key : keys) {
        los << Var::String(key) << delim;
    }
    os << String(los.str()).pop(1) << '\n';
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
        os << String(los.str()).pop(1) << std::endl;
    }
    return os;
}
std::ostream& Convert::ToCSV(std::ostream&& os, Var data, char delim) {
    return ToCSV(os, data, delim);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */