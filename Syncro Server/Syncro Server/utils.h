#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

class CStringLessThan {
	bool operator()(const std::string& inoLHS, const std::string& inoRHS) {
		return ( inoLHS.compare( inoRHS ) < 0 );
	}
};

#endif