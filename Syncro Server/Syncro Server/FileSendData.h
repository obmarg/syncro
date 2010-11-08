#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include <kode/utils.h>
#include <fstream>
#include <boost/shared_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <google/protobuf/io/zero_copy_stream.h>

namespace syncro {

class CFileSendData : boost::noncopyable {
public:
	CFileSendData(const std::string& insFilename,int innRequestedBufferSize=0);
	CFileSendData( CFileSendData& inoOther );
	~CFileSendData();

	void FillBuffer( google::protobuf::io::ZeroCopyOutputStream& stream );
	unsigned int GetChunkSize();

	unsigned int GetFilePosition();
	unsigned int GetFileSize() { return m_nFileSize; };
	bool IsFileFinished();
	bool IsStartFile();
	bool IsFileFinishedAfterChunk( unsigned int inNextChunkSize );

private:

	void OpenFile();

	std::ifstream m_oFile;
	const std::string m_sFilename;

	unsigned int	m_nFileSize;

	unsigned int m_nRequestedBufferSize;
};

} //end namespace syncro

#endif