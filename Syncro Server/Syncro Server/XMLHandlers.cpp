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

bool CXMLReceiveHandler::CanHandleReceive(const TCharBuffer& inoBuffer) {
	if(inoBuffer.nSize > 3) {
		if( (inoBuffer.aBuffer[0] == 'X') && (inoBuffer.aBuffer[1] == 'M') && (inoBuffer.aBuffer[2] == 'L') ) {
			if( inoBuffer.nSize > (3 + sizeof(long)) ) {
				long nExpectedSize = *((long*)&inoBuffer.aBuffer[3]);
			}
		}
	}
	return false;
}

bool CXMLReceiveHandler::HandleReceive(const TCharBuffer& inoBuffer) {
	//TODO: Do something
	return true;
}