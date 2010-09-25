#include "XMLHandlers.h"
#include "HandshakeHandlers.h"

CXMLAcceptHandler::CXMLAcceptHandler() : CAcceptHandler( 1 ) {

}

CXMLAcceptHandler::~CXMLAcceptHandler() {

}

bool CXMLAcceptHandler::HandleAccept(CTCPConnection::TPointer inpNewConnection) {
	inpNewConnection->AddRecvHandler( CHandshakeRecv::Create(inpNewConnection), 1 );
	inpNewConnection->StartRecv( 0 );
	return true;
}