#include "FileSendData.h"

namespace syncro {

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

CFileSendData::~CFileSendData() {
}

void CFileSendData::operator()(TCharBuffer::TBuff& inoBuffer) {

	char nFirstByte = FILE_SECTION_FIRST_BYTE;
	if( (m_oFile.tellg()) == std::ifstream::pos_type(0) ) {
		nFirstByte = FILE_SEND_FIRST_BYTE;
	}

	sHeader sDummyHeader(0,0,0);
	int nSizeOfHeader = sizeof( sDummyHeader.FirstByte ) + sizeof( sDummyHeader.Size ) + sizeof( sDummyHeader.FileOffset ) + sizeof( sDummyHeader.HashSize );
	
	//NOTE: sizeof(sHeader) is returning 16, when it should really be 10...
	//		so, this code is going to be a bit of a mess.  hence:
	//TODO: Clean this mess up
	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;	
	int nReadAmount = nBufferSize - nSizeOfHeader;
	int nSizeLeft = m_nFileSize - (int)m_oFile.tellg();
	nReadAmount = std::min( nReadAmount, nSizeLeft );
	inoBuffer.resize( nReadAmount + nSizeOfHeader );

	sHeader header(nFirstByte, inoBuffer.size(), 0 );
	//TODO: Has to be a better way of doing this shit:
	//		I.e. replace it with protocol buffers
	memcpy( &inoBuffer[0], &header.FirstByte, sizeof( header.FirstByte ) );
	memcpy( &inoBuffer[1], &header.Size, sizeof( header.Size ) );
	memcpy( &inoBuffer[5], &header.FileOffset, sizeof( header.FileOffset ) );
	memcpy( &inoBuffer[9], &header.HashSize, sizeof( header.HashSize ) );

	m_oFile.read( (char*)&inoBuffer[ sizeof( header ) ], nReadAmount );
	
	//DEBUG CODE:
	if( m_oFile.eof() || (m_oFile.tellg() == (std::streamoff)m_nFileSize) ) {
		//TODO: Not entirely sure why eof isn't returning true all the time.  Make sure i'm not fucking up the end of files.
		inoBuffer[0] = FILE_LAST_SECTION_FIRST_BYTE;
	}
}

bool CFileSendData::IsFileFinished() {
	return m_oFile.eof();
}

};		//namespace syncro