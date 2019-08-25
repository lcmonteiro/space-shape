
/**
 * ------------------------------------------------------------------------------------------------
 * File:   SPatern.cpp
 * Author: Luis Monteiro
 *
 * Created on Jun 27, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 ** 
 * std
 */
#include <list>
/**
 * space
 */
#include "SPattern.h"
/**
 * namespaces
 */
using namespace std; 
/**
 * utils
 */
#ifdef __DEBUG__
#include <iostream>
#endif
/**
 * ------------------------------------------------------------------------------------------------
 * Match - Implementation
 * ------------------------------------------------------------------------------------------------
 */
bool Tools::Pattern::Match(const String& value, const String& pattern) {
    /**
     * create and initialize boolean array to false.
     */
    vector<vector<bool>> test (
        value.size() + 1, 
        vector<bool>(pattern.size() + 1, false)
    );
	/**
     * base case
     */
	test[0][0] = true;
	/**
     * first line
     */
	for (size_t j = 0; j < pattern.size(); ++j) {
		if (pattern[j] == '*') {
			test[0][j + 1] = test[0][j];
		}
	}
	/**
     * remaining
     */ 
	for (size_t i = 0; i < value.size(); ++i) {
		for (size_t j = 0; j < pattern.size(); ++j) {
			if (value[i] == pattern[j] || pattern[j] == '?') {
				test[i + 1][j + 1] = test[i][j];
			} 
			else if (pattern[j] == '*') {
				test[i + 1][j + 1] = test[i + 1][j] || test[i][j + 1];
			}
		}
	}
#ifdef __DEBUG__	
	for(auto l : test) {
	    for(auto e : l)
	        cout << e << " "; 
	    cout << endl;
	}
#endif
	return test[value.size()][pattern.size()];
}
/**
 * ------------------------------------------------------------------------------------------------
 * Merge - Implementation
 * ------------------------------------------------------------------------------------------------
 */
String Tools::Pattern::Merge(const String& p1, const String& p2) {
	/**
     * create and initialize boolean array to false.
     */
    vector<vector<String>> cache (
        p1.size() + 1, 
        vector<String>(p2.size() + 1, "*")
	);
	/**
     * base case
     */
	cache[0][0] = "";
	/**
     * process the shortest path
     */ 
	for (size_t i = 0; i < p1.size(); ++i) {
		for (size_t j = 0; j < p2.size(); ++j) {
			auto diag = cache[i][j]; 
			auto top  = cache[i][j + 1]; 
			auto down = cache[i + 1][j]; 
			/**
			 * process diagonal
			 */
			if ('*'== p1[i] || '*'== p2[j]) {
				if('*' != diag.back()) { 
					diag.push_back('*');
				}
			} else { 
				if (p1[i] == p2[j]) {
					diag += p1[i];
				} else {
					diag += '?';
				}
			}
			/**
			 * process top & down
			 */
			if('*' != top.back()) { 
				top.push_back('*');
			}
			if('*' != down.back()) { 
				down.push_back('*');
			}
			/**
			 * select the minimum
			 */
			cache[i + 1][j + 1] = Minimum(
				diag, top, down
			);
		}
	}
#ifdef __DEBUG__	
	cout << endl;
	for(auto l : cache) {
	    for(auto e : l)
	        cout << e << "\t"; 
	    cout << endl;
	}
#endif
	return cache[p1.size()][p2.size()];
}
/**
 * ------------------------------------------------------------------------------------------------
 * Compare - Implementation
 * ------------------------------------------------------------------------------------------------
 */
String Tools::Pattern::Normalize(String p) {
	auto it_0 = p.begin();
	if(it_0 != p.end()) {
		
		auto it_1 = next(it_0);
		while(it_1 != p.end()) {
			if((*it_0 == *it_1) && ('*' == *it_0)) {
				it_1 = p.erase(it_1);
			} else {
				++it_0; ++it_1;
			}
		}
	}
	return move(p);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Compare - Implementation
 * ------------------------------------------------------------------------------------------------
 */
int Tools::Pattern::Compare(const String& p1, const String& p2) {
	/**
	 * p1 - counts
	 */
	size_t c1_0 = 0; 
	size_t c1_1 = 0;
	size_t c1_2 = 0;
	for(auto& c : p1) {
		if     ('*' == c) ++c1_0;
		else if('?' == c) ++c1_1;
		else 			  ++c1_2;
	}
	if('*' != p1.back() ) ++c1_2;
	if('*' != p1.front()) ++c1_2;
	if( 0  == c1_0      ) ++c1_0;
	/**
	 * p2 - counts
	 */
	size_t c2_0 = 0; 
	size_t c2_1 = 0;
	size_t c2_2 = 0;
	for(auto& c : p2) {
		if     ('*' == c) ++c2_0;
		else if('?' == c) ++c2_1;
		else 			  ++c2_2;
	}
	if('*' != p2.back() ) ++c2_2;
	if('*' != p2.front()) ++c2_2;
	if( 0  == c2_0     )  ++c2_0;
	/**
	 * diffs
	 */
	auto d_0 = c1_0 - c2_0;
	auto d_1 = c1_1 - c2_1;
	auto d_2 = c1_2 - c2_2;
	/**
	 * results
	 */
	if(d_2) return -d_2;
	if(d_1) return -d_1;	
	if(d_0) return +d_0;	
	return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */