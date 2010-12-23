#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include <kode/utils.h>
#include <kode/stopwatch.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <fstream>

namespace syncro {

class CFileSendData : boost::noncopyable {
public:
	CFileSendData( 
		const std::string& insFilename, 
		int innRequestedBufferSize=0,
		const VoidCallback& completionCallback=VoidCallback()
		);

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
	
	void CallCompletionCallback();

	std::ifstream m_oFile;
	const std::string m_sFilename;

	unsigned int	m_nFileSize;

	unsigned int m_nRequestedBufferSize;

	kode::time::Stopwatch m_stopwatch;
	VoidCallback m_completionCallback;

	bool m_finishedAfterChunk;
};

} //end namespace syncro

#endif