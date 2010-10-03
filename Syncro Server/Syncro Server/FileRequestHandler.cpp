//TODO: Check if this is a good idea
#pragma warning (disable: 4180)

#include "FileRequestHandler.h"

#include <string>
#include "Filesendhandler.h"
#include <boost/bind.hpp>

namespace syncro {

using std::string;

const unsigned char FILE_REQUEST_FIRST_BYTE = 10;
const unsigned char FILE_SECTION_REQUEST_FIRST_BYTE = 15;

CFileRequestHandler::CFileRequestHandler(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {
	m_fCloseConnection = false;
}

CFileRequestHandler::~CFileRequestHandler() {};

bool CFileRequestHandler::CanHandleReceive(const TCharBuffer& inoBuffer) {
	const int nHeaderLen = sizeof(char) + sizeof(unsigned int);
	if(inoBuffer.nSize > nHeaderLen ) {
		if( (inoBuffer.aBuffer[0] == FILE_REQUEST_FIRST_BYTE) || (inoBuffer.aBuffer[0] == FILE_SECTION_REQUEST_FIRST_BYTE) ) {
			unsigned int nExpectedSize = ( *(int*)(&inoBuffer.aBuffer[1]) );
			nExpectedSize = FromJavaEndian(nExpectedSize);
			if( inoBuffer.nSize >= nExpectedSize )
				return true;
		}
	}
	return false;
}

bool CFileRequestHandler::HandleReceive(const TCharBuffer& inoBuffer) {
	const int nHeaderLen = sizeof(char) + sizeof(unsigned int);

	string sFilename( inoBuffer.begin() + nHeaderLen, inoBuffer.end() );
	//TODO: Add folder number stuff in to this
	//TODO: also validate the incoming filename

	//But for now, just attempt to send the file
	if( inoBuffer.aBuffer[0] == FILE_REQUEST_FIRST_BYTE ) {
		m_pCurrentSend.reset( new CFileSendData( sFilename ) );
	}

	CFileSendHandler::TPointer oSender = CFileSendHandler::Create(m_pConn);
	//suspect when the above TPointer goes out of scope, the object will be deleted
	//TODO: need to figure out a way of transferring this stuff between shared_ptr types.  
	
	oSender->FillBuffer( m_pCurrentSend );
	m_pConn->Send( oSender );

	if( m_pCurrentSend->IsFileFinished() )
		m_pCurrentSend.reset();		//Release pointer

	return false;
}

}	//namespace syncro