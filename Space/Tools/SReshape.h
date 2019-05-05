/*
 * File:   SReshape.h
 * Author: Luis Monteiro
 *
 * Created on September 15, 2015, 5:54 PM
 */
#ifndef SRESHAPE_H
#define SRESHAPE_H
/**
 */
#include "Stypes/SVariables.h"
/**
 */
using namespace std;
/**
 */
typedef class SReshape {
public:
        /**
         * Map Reshape
         * @param doc
         * @param location
         * @param tags
         * @return reshape document
         */
        static Var toMap(Var doc, Key location, ListKey tags);
        /**
         * List Reshape
         * @param doc
         * @param location
         * @param tags
         * @return reshape document
         */
        static Var toList(Var doc, Key location);
} Reshape;
/**
 */
#endif /* SRESHAPE_H */

