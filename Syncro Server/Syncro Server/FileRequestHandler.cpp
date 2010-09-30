#include "FileRequestHandler.h"

#include <string>
#include "Filesendhandler.h"
#include <boost/bind.hpp>

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

	string sFilename( inoBuffer.begin() + nHeaderLen, inoBuffer.end() );
	//TODO: Add folder number stuff in to this
	//TODO: also validate the incoming filename

	//But for now, just attempt to send the file
	if( inoBuffer.aBuffer[0] == FILE_REQUEST_FIRST_BYTE ) {
		m_pCurrentSend.reset( new CFileSendData( sFilename ) );
	}

	CFileSendHandler::TPointer oSender = CFileSendHandler::Create(m_pConn);
		
	oSender->FillBuffer( boost::bind( (*m_pCurrentSend), &CFileSendData::FillBuffer ) );
	m_pConn->Send( CSendHandler::TPointer( oSender.get() ) );

	if( m_pCurrentSend->IsFileFinished() )
		m_pCurrentSend.reset();		//Release pointer

	return false;
}