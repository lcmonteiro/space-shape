/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   SBasic.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SMATH_H
#define SMATH_H
/**
 * std
 */
#include <algorithm>
#include <vector>
/**
 * ------------------------------------------------------------------------------------------------
 * Basic - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Tools {
namespace Math {
    /**
     * --------------------------------------------------------------------------------------------
     * Distance
     * --------------------------------------------------------------------------------------------
     * String
     * -----------------------------------------------------------------------
     */
    template<typename Type>
    size_t LevensteinDistance(const Type& source, const Type& target) {
        auto min_size = source.size(); 
        auto max_size = target.size();
        auto distance = std::vector<size_t>(min_size + 1);
        if (min_size > max_size) {
            return LevensteinDistance(target, source);
        }
        for (size_t i = 0; i <= min_size; ++i) {
            distance[i] = i;
        }
        for (size_t j = 1; j <= max_size; ++j) {
            auto previous_diagonal = distance.front()++;
            for (size_t i = 1; i <= min_size; ++i) {
                auto previous_diagonal_save = distance[i];
                if (source[i - 1] == target[j - 1]) {
                    distance[i] = previous_diagonal;
                } else {
                    distance[i] = std::min({distance[i - 1], distance[i], previous_diagonal}) + 1;
                }
                previous_diagonal = previous_diagonal_save;
            }
        }
        return distance[min_size];
    }
}}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SMATH_H */

