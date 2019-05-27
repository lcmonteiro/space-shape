
/*
 * File:   SEdit.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 */
#include <regex>
#include <iostream>
#include <list>
/**
 */
#include "SReshape.h"
#include "SEdit.h"
#include "SLogic.h"
#include "Sconverters/SConvert.h"
/**---------------------------------------------------------------------------------------------------------------------
 * map reshape
 **--------------------------------------------------------------------------------------------------------------------*/
Var SReshape::toMap(Var doc, Key location, KeyList tags) {
        Map map;
        for (Var e : Logic::toList(Edit::Find(location, doc))) {
                List l;
                for (Key k : tags) {
                        l.push_back(Edit::Find(k, e));
                }
                map[String::value_of(SConvert::toMD5(l))] = e;
        }
        return Edit::Insert(location, Obj(move(map)), doc);
}

Var SReshape::toList(Var doc, Key location){
        return doc;
}
