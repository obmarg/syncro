#include "FileSendHandler.h"
#include <fstream>

using std::string;
using std::ifstream;

const unsigned char FILE_SEND_FIRST_BYTE = 10;

CFileSendHandler::CFileSendHandler(CTCPConnection::TPointer inpConn,std::string insFilename) {
	using namespace std;

	ifstream oFile;
	oFile.open(insFilename,ios::in | ios::binary);
	if( oFile.is_open() ) {
		oFile.seekg( 0, ios::end );
		unsigned int nFileSize = (unsigned int)oFile.tellg();
		oFile.seekg( 0, ios::beg );
		unsigned int nTotalSize = nFileSize + sizeof( unsigned char ) + sizeof( unsigned int ); 
		m_aBuffer.resize( nTotalSize );
		m_aBuffer[0] = FILE_SEND_FIRST_BYTE;
		unsigned int* pInt = (unsigned int*)&m_aBuffer[1];
		(*pInt) = ToJavaEndian(nTotalSize);
		oFile.read( (char*)&m_aBuffer[ 1 + sizeof( unsigned int ) ], nFileSize);
	}
}

CFileSendHandler::~CFileSendHandler() {

}

void CFileSendHandler::SendDone(int innSent) {
	//Don't need to do anything here.
}