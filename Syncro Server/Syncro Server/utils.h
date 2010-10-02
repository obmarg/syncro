#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <boost\detail\endian.hpp>

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

template<typename T>
T ToJavaEndian(const T& source) {
#ifdef BOOST_BIG_ENDIAN
	return source;
#else
	T ret = 0;

	for (int i = 0; i < sizeof(T); ++i)
	{
		*(reinterpret_cast<char*>(&ret) + i) =
		*(reinterpret_cast<const char*>(&source) + sizeof(T) - i - 1);
	}
	return ret;
#endif
}

template<typename T>
T FromJavaEndian(const T& source) {
#ifdef BOOST_BIG_ENDIAN
	return source;
#else
	T ret = 0;

	for (int i = 0; i < sizeof(T); ++i)
	{
		*(reinterpret_cast<char*>(&ret) + i) =
		*(reinterpret_cast<const char*>(&source) + sizeof(T) - i - 1);
	}
	return ret;
#endif
}

#endif