/*
 * File:   SEdit.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 */

#ifndef SPATTERN_H
#define        SPATTERN_H
/**
 */
#include "Stypes/SVariables.h"
#include "Sbasis/SLog.h"
/**
 */
using namespace std;
/**
 */
typedef class SPattern {
public:
        /*-------------------------------------------------------------------------------------------------------------*
         * Create n combinations of patterns with format: prefix[0-9][2-9]sufix 
         *-------------------------------------------------------------------------------------------------------------*/
        static List Generate(Integer total, String sufix);
        //
        static List Generate(Integer total);
        //
        static Map Generate(List keys);
} Pattern;

#endif /* SPATTERN_H */

