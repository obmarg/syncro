#include "FileSendData.h"

#include <boost/numeric/conversion/cast.hpp>

namespace syncro {

const int DEFAULT_FILE_SEND_BUFFER_SIZE = 1024 * 1024;

const unsigned char FILE_SEND_FIRST_BYTE = 11;
const unsigned char FILE_SECTION_FIRST_BYTE = 16;
const unsigned char FILE_LAST_SECTION_FIRST_BYTE = 20;

CFileSendData::CFileSendData(
	const std::string& insFilename,
	const int innRequestedBufferSize,
	const VoidCallback& completionCallback
	) 
	: 
m_sFilename( insFilename ),
m_completionCallback( completionCallback ),
m_finishedAfterChunk( false )
{
	m_nRequestedBufferSize = innRequestedBufferSize;
	OpenFile();
}

CFileSendData::~CFileSendData() {

}

void CFileSendData::OpenFile() {
	using namespace std;
	m_oFile.open( m_sFilename.c_str() ,ios::in | ios::binary);
	if( m_oFile.is_open() ) {
		m_oFile.seekg( 0, ios::end );
		m_nFileSize = (unsigned int)m_oFile.tellg();
		m_oFile.seekg( 0, ios::beg );
	} else
		throw std::runtime_error( "CFileSendData called on non existant file" );
}

void CFileSendData::FillBuffer(google::protobuf::io::ZeroCopyOutputStream& stream) {
	int nReadAmount = GetChunkSize();

	void *pData;
	int nSize;
	while( nReadAmount > 0 ) {
		if( !stream.Next( &pData, &nSize ) )
			throw std::runtime_error( "ZeroCopyOutputStream returned false in CFileSendData::FillBuffer" );
		char* pChars = reinterpret_cast< char* >( pData );
		//TODO: Maybe start using google's fileinputstream to read data?
		m_oFile.read( pChars, nSize );
		nReadAmount -= nSize;
	}
	if( m_finishedAfterChunk && m_completionCallback )
	{
		m_completionCallback();
		m_completionCallback.clear();
	}
}

unsigned int CFileSendData::GetChunkSize() {
	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	if( m_nRequestedBufferSize != 0 ) {
		int nRequestedBufferSize = (m_nRequestedBufferSize - 1024);		//Take away 1k just to be safe?
		nBufferSize = std::min( DEFAULT_FILE_SEND_BUFFER_SIZE, nRequestedBufferSize );
	}
	//TODO: maybe stop doing the above -1024, and find a better way of doing it
	int nSizeLeft = m_nFileSize - (int)m_oFile.tellg();
	return std::min( nBufferSize, nSizeLeft );
}

bool CFileSendData::IsStartFile() {
	if( m_oFile.tellg() == (std::streamoff)0 )
		return true;
	return false;
}

bool CFileSendData::IsFileFinished() {
	using boost::numeric_cast;

	if( m_oFile.tellg() == (std::streamoff)m_nFileSize || m_oFile.eof() )
	{
		int64_t timeMs = m_stopwatch.GetMS();
		float dataTransferred = 
			numeric_cast<float>( GetFilePosition() ) / 1024.0f;

		float timeSecs = numeric_cast<float>( timeMs ) / 1000.0f; 

		float rate = dataTransferred / timeSecs;
		std::cout << "Finished sending file (" << rate << "kb/s)\n";

		if( m_completionCallback )
		{
			m_completionCallback();
			m_completionCallback.clear();
		}

		return true;
	}
	return false;
}

unsigned int CFileSendData::GetFilePosition() {
	return (unsigned int)m_oFile.tellg();
}

bool CFileSendData::IsFileFinishedAfterChunk( unsigned int inNextChunkSize ) {
	std::streamoff nNextTell = m_oFile.tellg() + (std::streamoff)inNextChunkSize;
	if( nNextTell == (std::streamoff)m_nFileSize ) {
		m_finishedAfterChunk = true;
		return true;
	}
	return false;
}

};		//namespace syncro
