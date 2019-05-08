/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SCsvConvert.h
 * Author: João Lima
 *
 * Created on Apr 10, 2019, 12:11 PM
 */

#ifndef SCSVCONVERT_H
#define SCSVCONVERT_H
/**
 */
#include <sstream>
#include <iostream>
/**
 */
#include "Stools/SEdit.h"
#include "Stypes/SVariables.h"
#include "Stools/SUtils.h"
/**
 */
typedef class __Scsvconvert__ {
public:
        /*-------------------------------------------------------------------------------------------------------------*
         *  from CSV
         *-------------------------------------------------------------------------------------------------------------*/
        static Var fromCSV(std::istream& v, char delim = ',');
        /**
         * alternatives
         */
        static inline Var fromCSV(std::istream&& is, char delim = ',') {
                return fromCSV(is); 
        }
        /*-------------------------------------------------------------------------------------------------------------*
         *  to CSV
         *-------------------------------------------------------------------------------------------------------------*/
        static std::ostream& toCSV(std::ostream& os, Var data, char delim = ',');
} CsvConvert;

#endif /* SCSVCONVERT_H */

