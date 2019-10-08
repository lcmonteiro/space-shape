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
    namespace Base {
        /**
         * base algoritm LevensteinDistance
         */
        template<typename Type>
        size_t Distance(const Type& source, const Type& target) {
            auto distance = std::vector<size_t>(source.size() + 1);
            for (size_t i = 0; i <= source.size(); ++i) {
                distance[i] = i;
            }
            for (size_t j = 1; j <= target.size(); ++j) {
                auto previous_diagonal = distance.front()++;
                for (size_t i = 1; i <= source.size(); ++i) {
                    auto previous_diagonal_save = distance[i];
                    if (source[i - 1] == target[j - 1]) {
                        distance[i] = previous_diagonal;
                    } else {
                        distance[i] = std::min({distance[i - 1], distance[i], previous_diagonal}) + 1;
                    }
                    previous_diagonal = previous_diagonal_save;
                }
            }
            return distance[source.size()];
        }
    }
    template<typename Type>
    size_t Distance(const Type& source, const Type& target) {
        if (source.size() > target.size()) {
            return Distance(target, source);
        }
        return Base::Distance(source, target);
    }
    template<typename Type>
    float_t DistanceNormalized(const Type& source, const Type& target) {
        if (source.size() > target.size()) {
            return DistanceNormalized(target, source);
        }
        return float_t(Base::Distance(source, target)) / target.size();
    }
}}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SMATH_H */

