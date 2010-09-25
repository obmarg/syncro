#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

class CStringLessThan {
public:
	bool operator()(const std::string& inoLHS, const std::string& inoRHS) {
		return ( inoLHS.compare( inoRHS ) < 0 );
	}
};

class CPointerLessThan {
public:
	template<class tPointerType>
	bool operator()(const tPointerType& inoLHS, const tPointerType& inoRHS) {
		return ( (*inoLHS) < (*inoRHS) );
	}
};

#endif