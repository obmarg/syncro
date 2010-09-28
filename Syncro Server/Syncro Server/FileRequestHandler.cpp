#include "FileRequestHandler.h"

#include <string>

using std::string;

const unsigned char FILE_REQUEST_FIRST_BYTE = 10;

CFileRequestHandler::CFileRequestHandler(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {
	m_fCloseConnection = false;
}

CFileRequestHandler::~CFileRequestHandler() {};

bool CFileRequestHandler::CanHandleReceive(const TCharBuffer& inoBuffer) {
	const int nHeaderLen = sizeof(char) + sizeof(unsigned int);
	if(inoBuffer.nSize > nHeaderLen ) {
		if( inoBuffer.aBuffer[0] == FILE_REQUEST_FIRST_BYTE  ) {
			//TODO: Convert byte buffer to unsigned int (endianness matters too)
			unsigned int nExpectedSize = inoBuffer.aBuffer[1];
			if( inoBuffer.nSize > (unsigned int)nExpectedSize )
				return true;
		}
	}
	return false;
}

bool CFileRequestHandler::HandleReceive(const TCharBuffer& inoBuffer) {
	const int nHeaderLen = sizeof(char) + sizeof(unsigned int);

	string sFilename( inoBuffer.begin() + 2, inoBuffer.end() );
	//TODO: Add folder number stuff in to this
	//TODO: also validate the incoming filename

	//But for now, just attempt to send the file

	return false;
}