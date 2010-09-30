#include "FileSendData.h"

const int DEFAULT_FILE_SEND_BUFFER_SIZE = 1024;

const unsigned char FILE_SEND_FIRST_BYTE = 11;
const unsigned char FILE_SECTION_FIRST_BYTE = 16;
const unsigned char FILE_LAST_SECTION_FIRST_BYTE = 20;

CFileSendData::CFileSendData(const std::string& insFilename) {
	using namespace std;
	m_oFile.open(insFilename,ios::in | ios::binary);
	if( m_oFile.is_open() ) {
		m_oFile.seekg( 0, ios::end );
		m_nFileSize = (unsigned int)m_oFile.tellg();
		m_oFile.seekg( 0, ios::beg );
	}
}

void CFileSendData::FillBuffer(TCharBuffer::TBuff& inoBuffer) {

	char inFirstByte = FILE_SECTION_FIRST_BYTE;
	if( (m_oFile.tellg()) == std::ifstream::pos_type(0) ) {
		inFirstByte = FILE_SEND_FIRST_BYTE;
	}
	
	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	int inReadAmount = nBufferSize - sizeof(sHeader);
	int inSizeLeft = m_nFileSize - (int)m_oFile.tellg();
	inReadAmount = std::min( inReadAmount, inSizeLeft );
	inoBuffer.resize( inReadAmount + sizeof(sHeader) );

	sHeader header(inFirstByte, inoBuffer.size(), 0 );
	memcpy( &inoBuffer[0], &header, sizeof( header ) );

	m_oFile.read( (char*)&inoBuffer[ sizeof( header ) ], inReadAmount );
	if( m_oFile.eof() ) {
		inoBuffer[0] = FILE_LAST_SECTION_FIRST_BYTE;
	}
}

bool CFileSendData::IsFileFinished() {
	return m_oFile.eof();
}