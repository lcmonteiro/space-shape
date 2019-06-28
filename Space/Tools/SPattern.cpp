
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
#include <vector>
/**
 * space
 */
#include "SPattern.h"
/**
 * namespaces
 */
using namespace std; 
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
	for (size_t j = 1; j <= pattern.size(); j++) {
		if (pattern[j-1] == '*') {
			test[0][j] = test[0][j-1];
		}
	}
	/**
     * remaining
     */ 
	for (size_t i = 1; i <= value.size(); i++) {
		for (size_t j = 1; j <= pattern.size(); j++) {
			if (value[i-1] == pattern[j-1] || pattern[j-1] == '?') {
				test[i][j] = test[i-1][j-1];
			} 
			else if (pattern[j-1] == '*') {
				test[i][j] = test[i][j-1] || test[i-1][j];
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
 * End
 * ------------------------------------------------------------------------------------------------
 */