
/*
 * File:   SEdit.cc
 * Author: Luis Monteiro
 *
 * Created on February 3, 2016, 5:54 PM
 */
#include <algorithm>
/**
 */
#include "SPattern.h"
#include "SUtils.h"
/*---------------------------------------------------------------------------------------------------------------------*
 * find path
 *---------------------------------------------------------------------------------------------------------------------*/
static int __div(int value){
        static const std::vector<int> factors = {
                10, 9, 8, 6, 7, 5, 4, 3, 2
        };
        for (auto f : factors) {
                if (value % f == 0) {
                        return f;
                }
        }
        return 0;
}
static int __find(int value, std::vector<int>& path) {
        if (!value) {
                return 0;
        }
        /**
         * minimize
         */
        auto value_r = value;
        auto value_c = 1;
        for (auto d = __div(value_r); d > 1; d = __div(value_r)) {
                path.push_back(d);
                value_r /= d;
                value_c *= d;
        }
        /**
         * split
         */
        if(value_r > 1){
                auto path_p = path;
                auto path_n = move(path);
                /**
                 */
                auto value_p = __find(value_r + 1, path_p);
                auto value_n = __find(value_r - 1, path_n);
                /**
                 */
                if (path_p.size() < path_n.size()) {
                        path = move(path_p);
                        return value_c * value_p;
                }
                if (path_p.size() > path_n.size()) {
                        path = move(path_n);
                        return value_c * value_n;
                }
                /**
                 */
                if (abs(value_p - value) < abs(value_n - value)) {
                        path = move(path_p);
                        return value_c * value_p;
                }
                /**
                 */        
                path = move(path_n);
                return value_c * value_n;
        }
        return value_c;
}
/*---------------------------------------------------------------------------------------------------------------------*
 * simplify
 *---------------------------------------------------------------------------------------------------------------------*/
static vector<int> __simplify(vector<int>&& lst) {
        /**
         * sort sequence
         */
        sort(lst.begin(), lst.end());
        /**
         * compress sequence
         */
        vector<int> s;
        int tmp = 1;
        for (auto l : lst) {
                if (tmp * l > 10) {
                        s.push_back(tmp);
                        tmp = l;
                } else {
                        tmp *= l;
                }
        }
        s.push_back(tmp);
        /**
         * sort sequence
         */
        std::sort(s.begin(), s.end());
        return s;
}
/*---------------------------------------------------------------------------------------------------------------------*
 * create pattern
 *---------------------------------------------------------------------------------------------------------------------*/
static vector<String> __add(int weitht, vector<String>&& carrier) {
        static const vector< vector<Integer> > key = {
                {},
                {10},
                {5, 5},
                {4, 3, 3},
                {3, 3, 2, 2},
                {2, 2, 2, 2, 2},
                {2, 2, 2, 2, 1, 1},
                {2, 2, 2, 1, 1, 1, 1},
                {2, 2, 1, 1, 1, 1, 1, 1},
                {2, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };
        /**
         * append expression
         */
        vector<String> result;
        Integer pointer;
        for (auto k : key[weitht]) {
                for (auto& c : carrier) {
                        result.push_back(
                                "[" + String::value_of(pointer) + "-" + String::value_of(pointer + k - 1) + "]" + c
                        );
                }
                pointer += k;
        }
        return result;
}
static vector<String> __create(const vector<int>& weights) {
        vector<String> result {
                ""
        };
        for (auto it = weights.rbegin(), end = weights.rend(); it != end; ++it){
                result = __add(*it, move(result));
        }
        return result;
}
/*---------------------------------------------------------------------------------------------------------------------*
 * fit pattern size to total
 *---------------------------------------------------------------------------------------------------------------------*/
static vector<String> __fit(size_t total, vector<String>&& pattern) {        
        for (; total < pattern.size(); pattern.pop_back()) {
                pattern.front() += (string(")|(") + pattern.back());
        }
        for(auto& p: pattern) {
                p = "(" + p + ")";
        }
        for (auto i = pattern.size(); i < total; ++i) {
               pattern.push_back("");
        }
        return pattern;
}

/*---------------------------------------------------------------------------------------------------------------------*
 * Interfaces
 *---------------------------------------------------------------------------------------------------------------------*/
List SPattern::Generate(Integer total) {
        vector<int> path;
        //*
        if (__find(total, path) == 0) {
                return {};
        }
        //*
        List result;
        for(auto& p : __fit(total, __create(__simplify(move(path))))){
                result.push_back(Obj(move(p)));
        }
        return result;
}

Map SPattern::Generate(List keys) {
        // get patterns
        auto patterns = Generate(keys.size());
        // sort
        sort(keys.begin(), keys.end(), [](Var a, Var b) {
                return String(a) < String(b);
        });
        // combine key:value
        return Utils::combine(keys, patterns);
}

List SPattern::Generate(Integer total, String sufix) {
        // get patterns
        auto patterns = Generate(total);
        // update with sufix
        for (auto& p : patterns) {
                Var::String(p) += sufix;
        }
        // updated patterns
        return patterns;
}