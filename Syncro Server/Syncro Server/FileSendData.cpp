#include "FileSendData.h"

namespace syncro {

const int DEFAULT_FILE_SEND_BUFFER_SIZE = 1024;

const unsigned char FILE_SEND_FIRST_BYTE = 11;
const unsigned char FILE_SECTION_FIRST_BYTE = 16;
const unsigned char FILE_LAST_SECTION_FIRST_BYTE = 20;

CFileSendData::CFileSendData(const std::string& insFilename) : m_sFilename(insFilename) {
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
	}
}

void CFileSendData::FillBuffer(TCharBuffer::TBuff& inoBuffer) {

	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	int nSizeLeft = m_nFileSize - (int)m_oFile.tellg();
	int nReadAmount = std::min( nBufferSize, nSizeLeft );
	inoBuffer.resize( nReadAmount );

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