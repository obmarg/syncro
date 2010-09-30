#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include "utils.h"
#include <fstream>

class CFileSendData {
public:
	typedef void result_type;

	CFileSendData(const std::string& insFilename);
	~CFileSendData();

	void FillBuffer(TCharBuffer::TBuff& inoBuffer);

	bool IsFileFinished();

private:
	struct sHeader {
	public:
		sHeader( char inFirstByte, int inSize, unsigned char inFileOffset ) {
			FirstByte = inFirstByte;
			Size = ToJavaEndian(inSize);
			FileOffset = ToJavaEndian(inFileOffset);
			HashSize = 0;
		}
		unsigned char FirstByte;
		unsigned int Size;
		unsigned long FileOffset;
		unsigned char HashSize;
		//insert hash here
	};

	std::ifstream m_oFile;

	unsigned int	m_nFileSize;
};

#endif