#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include "utils.h"
#include <fstream>
#include <boost/shared_ptr.hpp>

#include <boost/noncopyable.hpp>

namespace syncro {

class CFileSendData {
public:
	typedef boost::shared_ptr<CFileSendData> TPointer;

	CFileSendData(const std::string& insFilename);
	CFileSendData( const CFileSendData& ) {
	}
	~CFileSendData();

	void operator()(TCharBuffer::TBuff& inoBuffer);

	bool IsFileFinished();

private:

	struct sHeader {
	public:
		sHeader( unsigned char inFirstByte, unsigned int inSize, unsigned int inFileOffset ) {
			FirstByte = inFirstByte;
			Size = ToJavaEndian(inSize);
			FileOffset = ToJavaEndian(inFileOffset);
			HashSize = 0;
		};
		unsigned char FirstByte;
		unsigned int Size;
		unsigned int FileOffset;
		unsigned char HashSize;
		//insert hash here
	};

	std::ifstream m_oFile;

	unsigned int	m_nFileSize;
};

} //end namespace syncro

#endif