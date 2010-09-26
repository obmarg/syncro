#ifndef _SYNCRO_COMMON_H_
#define _SYNCRO_COMMON_H_

#include <vector>

struct TCharBuffer {
	typedef unsigned char TChar;
	typedef std::vector<TChar> TBuff;
	TCharBuffer(TBuff& inoBuff, unsigned int innSize) : aBuffer(inoBuff), nSize( innSize ) {};

	TBuff& aBuffer;
	unsigned int nSize;
};

#endif