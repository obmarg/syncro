#ifndef _SYNCRO_COMMON_H_
#define _SYNCRO_COMMON_H_

#include <vector>

namespace syncro {

struct TCharBuffer {
	typedef unsigned char TChar;
	typedef std::vector<TChar> TBuff;
	TCharBuffer(TBuff& inoBuff, unsigned int innSize) : aBuffer(inoBuff), nSize( innSize ) {};
	
	TBuff::iterator begin() { return aBuffer.begin(); };
	TBuff::iterator end() { return aBuffer.begin() + nSize; };

	TBuff::const_iterator begin() const { return aBuffer.begin(); };
	TBuff::const_iterator end() const { return aBuffer.begin() + nSize; };

	TBuff& aBuffer;
	unsigned int nSize;
};

}; //namespace syncro

#endif