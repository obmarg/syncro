#include "FileSendData.h"

namespace syncro {

const int DEFAULT_FILE_SEND_BUFFER_SIZE = 1024 * 1024;

const unsigned char FILE_SEND_FIRST_BYTE = 11;
const unsigned char FILE_SECTION_FIRST_BYTE = 16;
const unsigned char FILE_LAST_SECTION_FIRST_BYTE = 20;

CFileSendData::CFileSendData(const std::string& insFilename,const int innRequestedBufferSize) : m_sFilename(insFilename) {
	m_nRequestedBufferSize = innRequestedBufferSize;
	OpenFile();
}

CFileSendData::CFileSendData( CFileSendData& inoOther ) : m_sFilename(inoOther.m_sFilename) {
	OpenFile();
	m_oFile.seekg( inoOther.m_oFile.tellg() );
}

CFileSendData::~CFileSendData() {

}

void CFileSendData::OpenFile() {
	using namespace std;
	m_oFile.open( m_sFilename ,ios::in | ios::binary);
	if( m_oFile.is_open() ) {
		m_oFile.seekg( 0, ios::end );
		m_nFileSize = (unsigned int)m_oFile.tellg();
		m_oFile.seekg( 0, ios::beg );
	} else
		throw std::exception( "CFileSendData called on non existant file" );
}

void CFileSendData::FillBuffer(TCharBuffer::TBuff& inoBuffer) {

	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	if( m_nRequestedBufferSize != 0 ) {
		int nRequestedBufferSize = (m_nRequestedBufferSize - 1024);		//Take away 1k just to be safe?
		nBufferSize = std::min( DEFAULT_FILE_SEND_BUFFER_SIZE, nRequestedBufferSize );
	}
	//TODO: maybe stop doing the above -1024, and find a better way of doing it
	int nSizeLeft = m_nFileSize - (int)m_oFile.tellg();
	int nReadAmount = std::min( nBufferSize, nSizeLeft );
	inoBuffer.resize( nReadAmount );

	if( nReadAmount != 0 )
		m_oFile.read( (char*)&inoBuffer[ 0 ], nReadAmount );
}

bool CFileSendData::IsStartFile() {
	if( m_oFile.tellg() == (std::streamoff)0 )
		return true;
	return false;
}

bool CFileSendData::IsFileFinished() {
	if( m_oFile.tellg() == (std::streamoff)m_nFileSize )
		return true;
	return m_oFile.eof();
}

unsigned int CFileSendData::GetFilePosition() {
	return (unsigned int)m_oFile.tellg();
}

};		//namespace syncro