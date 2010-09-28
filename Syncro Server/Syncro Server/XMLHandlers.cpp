#include "XMLHandlers.h"
#include "XMLBuilder.h"
#include "HandshakeHandlers.h"
#include "ServerComms.h"
#include <string>
#include <boost/lexical_cast.hpp>

using std::string;

const unsigned char XML_REQUEST_FIRST_BYTE = 5;
const unsigned char XML_RESPONSE_FIRST_BYTE = 6;

CXMLAcceptHandler::CXMLAcceptHandler() : CAcceptHandler( 1 ) {

}

CXMLAcceptHandler::~CXMLAcceptHandler() {

}

bool CXMLAcceptHandler::HandleAccept(CTCPConnection::TPointer inpNewConnection) {
	inpNewConnection->AddRecvHandler( CHandshakeRecv::Create(inpNewConnection), 1 );
	inpNewConnection->StartRecv( 0 );
	return true;
}

CXMLReceiveHandler::CXMLReceiveHandler(CTCPConnection::TPointer inpConn) : m_pConn( inpConn ) {
	m_fCloseConnection = false;
}

bool CXMLReceiveHandler::CanHandleReceive(const TCharBuffer& inoBuffer) {
	if(inoBuffer.nSize > 2) {
		if( inoBuffer.aBuffer[0] == XML_REQUEST_FIRST_BYTE  ) {
			unsigned char nExpectedSize = inoBuffer.aBuffer[1];
			if( inoBuffer.nSize > (unsigned int)nExpectedSize )
				return true;
		}
	}
	return false;
}

bool CXMLReceiveHandler::HandleReceive(const TCharBuffer& inoBuffer) {
	
	unsigned char nExpectedSize = inoBuffer.aBuffer[1];
	const string sGetFolderList = "GET_FOLDER_LIST";
	const string sGetFolderContents = "GET_FOLDER_CONTENTS:";

	string sBuffer( inoBuffer.begin() + 2, inoBuffer.end() );
	if( sBuffer.compare(sGetFolderList) == 0 ) {
		//Requesting folder list
		if( m_oXMLBuilder.GetFolderXML() ) {
			CXMLSendHandler::TPointer pSender = CXMLSendHandler::Create(m_pConn);
			pSender->CreateXML( CXMLOutput( m_oXMLBuilder ) );
			m_pConn->Send( pSender );
		} else
			throw std::exception("Failed to get folder xml\n");
	} 
	else if( sBuffer.substr( 0, sGetFolderContents.length() ).compare( sGetFolderContents ) == 0 )
	{
		if( m_oXMLBuilder.GetFolderContentsXML( boost::lexical_cast<int>( sBuffer.substr( sGetFolderContents.length(),  sBuffer.length() - sGetFolderContents.length() ) ) ) ) {
			CXMLSendHandler::TPointer pSender = CXMLSendHandler::Create(m_pConn);
			pSender->CreateXML( CXMLOutput( m_oXMLBuilder ) );
			m_pConn->Send( pSender );
		}
	}
	return true;
}

CXMLSendHandler::CXMLSendHandler(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {
	m_aBuffer.push_back( XML_RESPONSE_FIRST_BYTE );
	//Add space for the legnth of the message, which will be added after XML
	m_aBuffer.push_back( 0 );	
	m_aBuffer.push_back( 0 );
	m_aBuffer.push_back( 0 );
	m_aBuffer.push_back( 0 );
}

CXMLSendHandler::~CXMLSendHandler() {

}

void CXMLSendHandler::SendDone(int innSent) {
	m_pConn->StartRecv(0);
}
