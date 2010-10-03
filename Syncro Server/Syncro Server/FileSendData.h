#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include "utils.h"
#include <fstream>
#include <boost/shared_ptr.hpp>

#include <boost/noncopyable.hpp>

namespace syncro {

class CFileSendData : boost::noncopyable {
public:
	CFileSendData(const std::string& insFilename);
	CFileSendData( CFileSendData& inoOther );
	~CFileSendData();

	void FillBuffer(TCharBuffer::TBuff& inoBuffer);

	unsigned int GetFilePosition();
	unsigned int GetFileSize() { return m_nFileSize; };
	bool IsFileFinished();
	bool IsStartFile();

private:

	void OpenFile();

	std::ifstream m_oFile;
	const std::string m_sFilename;

	unsigned int	m_nFileSize;
};

} //end namespace syncro

#endif